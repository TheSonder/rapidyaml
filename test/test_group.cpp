#include "./test_group.hpp"

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable: 4127/*conditional expression is constant*/)
#   pragma warning(disable: 4389/*'==': signed/unsigned mismatch*/)
#endif

#include <yaml-cpp/yaml.h>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

namespace c4 {
namespace yml {

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
TEST_P(YmlTestCase, parse_using_libyaml)
{
    try
    {
        return; // disable this for now
        Tree t;
        LibyamlParser libyaml_parser;
        libyaml_parser.parse(&t, c->src);
    }
    catch(...)
    {
        if(c->flags & IGNORE_LIBYAML_PARSE_FAIL)
        {
            std::cout << "libyaml failed parsing; ignoring\n";
        }
        else
        {
            std::cout << "libyaml failed parsing the following source:\n";
            std::cout << "---------------\n";
            std::cout << c->src;
            std::cout << "---------------\n";
            throw;
        }
    }
}

//-----------------------------------------------------------------------------
TEST_P(YmlTestCase, parse_using_yaml_cpp)
{
    try
    {
        std::string src(c->src.str, c->src.len);
        YAML::Node node = YAML::Load(src);
    }
    catch(...)
    {
        if(c->flags & IGNORE_YAMLCPP_PARSE_FAIL)
        {
            std::cout << "yamlcpp failed parsing the following source:\n";
        }
        else
        {
            std::cout << "yamlcpp failed parsing the following source:\n";
            std::cout << "---------------\n";
            std::cout << c->src;
            std::cout << "---------------\n";
            throw;
        }
    }
}

//-----------------------------------------------------------------------------
TEST_P(YmlTestCase, parse_using_ryml)
{
#ifdef RYML_DBG
    std::cout << "---------------\n";
    std::cout << c->src;
    std::cout << "---------------\n";
#endif
    parse(d->src, &d->parsed_tree);
    {
        SCOPED_TRACE("checking tree invariants of unresolved parsed tree");
        check_invariants(d->parsed_tree);
    }
#ifdef RYML_DBG
    print_tree(c->root);
    print_tree(d->parsed_tree);
#endif
    {
        SCOPED_TRACE("checking node invariants of unresolved parsed tree");
        check_invariants(d->parsed_tree.rootref());
    }

    if(c->flags & RESOLVE_REFS)
    {
        d->parsed_tree.resolve();
#ifdef RYML_DBG
        std::cout << "resolved tree!!!\n";
        print_tree(d->parsed_tree);
#endif
        {
            SCOPED_TRACE("checking tree invariants of resolved parsed tree");
            check_invariants(d->parsed_tree);
        }
        {
            SCOPED_TRACE("checking node invariants of resolved parsed tree");
            check_invariants(d->parsed_tree.rootref());
        }
    }

    {
        SCOPED_TRACE("comparing parsed tree to ref tree");
        EXPECT_GE(d->parsed_tree.capacity(), c->root.reccount());
        EXPECT_EQ(d->parsed_tree.size(), c->root.reccount());
        c->root.compare(d->parsed_tree.rootref());
    }
}

//-----------------------------------------------------------------------------
TEST_P(YmlTestCase, emit_yml_stdout)
{
    d->numbytes_stdout = emit(d->parsed_tree);
}

//-----------------------------------------------------------------------------
TEST_P(YmlTestCase, emit_yml_cout)
{
    std::cout << d->parsed_tree;
}

//-----------------------------------------------------------------------------
TEST_P(YmlTestCase, emit_yml_stringstream)
{
    std::string s;
    std::vector<char> v;
    csubstr sv = emitrs(d->parsed_tree, &v);

    {
        std::stringstream ss;
        ss << d->parsed_tree;
        s = ss.str();
        EXPECT_EQ(sv, s);
    }

    {
        std::stringstream ss;
        ss << d->parsed_tree.rootref();
        s = ss.str();

        csubstr sv = emitrs(d->parsed_tree, &v);
        EXPECT_EQ(sv, s);
    }
}

//-----------------------------------------------------------------------------
TEST_P(YmlTestCase, emit_yml_string)
{
    auto em = emitrs(d->parsed_tree, &d->emit_buf);
    EXPECT_EQ(em.len, d->emit_buf.size());
    EXPECT_EQ(em.len, d->numbytes_stdout);
    d->emitted_yml = em;

#ifdef RYML_DBG
    std::cout << em;
#endif
}

TEST_P(YmlTestCase, emitrs)
{
    using vtype = std::vector<char>;
    using stype = std::string;

    vtype vv, v = emitrs<vtype>(d->parsed_tree);
    stype ss, s = emitrs<stype>(d->parsed_tree);
    EXPECT_EQ(to_csubstr(v), to_csubstr(s));

    csubstr svv = emitrs(d->parsed_tree, &vv);
    csubstr sss = emitrs(d->parsed_tree, &ss);
    EXPECT_EQ(svv, sss);
}

//-----------------------------------------------------------------------------
TEST_P(YmlTestCase, complete_round_trip)
{
    if(d->parsed_tree.empty())
    {
        parse(d->src, &d->parsed_tree);
    }
    if(d->emit_buf.empty())
    {
        d->emitted_yml = emitrs(d->parsed_tree, &d->emit_buf);
    }

#ifdef RYML_DBG
    print_tree(d->parsed_tree);
    std::cout << d->emitted_yml;
#endif

    {
        SCOPED_TRACE("parsing emitted yml");
        d->parse_buf = d->emit_buf;
        d->parsed_yml.assign(d->parse_buf.data(), d->parse_buf.size());
        parse(d->parsed_yml, &d->emitted_tree);
#ifdef RYML_DBG
        print_tree(d->emitted_tree);
#endif
    }

    {
        SCOPED_TRACE("checking node invariants of parsed tree");
        check_invariants(d->emitted_tree.rootref());
    }

    {
        SCOPED_TRACE("checking tree invariants of parsed tree");
        check_invariants(d->emitted_tree);
    }

    {
        SCOPED_TRACE("comparing parsed tree to ref tree");
        EXPECT_GE(d->emitted_tree.capacity(), c->root.reccount());
        EXPECT_EQ(d->emitted_tree.size(), c->root.reccount());
        c->root.compare(d->emitted_tree.rootref());
    }
}

//-----------------------------------------------------------------------------
TEST_P(YmlTestCase, recreate_from_ref)
{
    if(d->parsed_tree.empty())
    {
        parse(d->src, &d->parsed_tree);
    }
    if(d->emit_buf.empty())
    {
        d->emitted_yml = emitrs(d->parsed_tree, &d->emit_buf);
    }

    {
        SCOPED_TRACE("recreating a new tree from the ref tree");
        d->recreated.reserve(d->parsed_tree.size());
        NodeRef r = d->recreated.rootref();
        c->root.recreate(&r);
    }

    {
        SCOPED_TRACE("checking node invariants of recreated tree");
        check_invariants(d->recreated.rootref());
    }

    {
        SCOPED_TRACE("checking tree invariants of recreated tree");
        check_invariants(d->recreated);
    }

    {
        SCOPED_TRACE("comparing recreated tree to ref tree");
        c->root.compare(d->recreated.rootref());
    }
}

} // namespace yml
} // namespace c4
