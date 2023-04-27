project "ryml"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/c4/**.hpp",
		"src/c4/**.cpp",
		"src/ryml.hpp",
        "src/ryml_std.hpp",
        "src/c4/yml/detail/checks.hpp",
        "src/c4/yml/detail/parser_dbg.hpp",
        "src/c4/yml/detail/print.hpp",
        "src/c4/yml/detail/stack.hpp",
        "src/c4/yml/common.hpp",
        "src/c4/yml/common.cpp",
        "src/c4/yml/emit.def.hpp",
        "src/c4/yml/emit.hpp",
        "src/c4/yml/export.hpp",
        "src/c4/yml/node.hpp",
        "src/c4/yml/node.cpp",
        "src/c4/yml/parse.hpp",
        "src/c4/yml/parse.cpp",
        "src/c4/yml/preprocess.hpp",
        "src/c4/yml/preprocess.cpp",
        "src/c4/yml/std/map.hpp",
        "src/c4/yml/std/std.hpp",
        "src/c4/yml/std/string.hpp",
        "src/c4/yml/std/vector.hpp",
        "src/c4/yml/tree.hpp",
        "src/c4/yml/tree.cpp",
        "src/c4/yml/writer.hpp",
        "src/c4/yml/yml.hpp",
        "src/ryml.natvis",
		"ext/c4core/src/c4/allocator.hpp",
        "ext/c4core/src/c4/base64.hpp",
        "ext/c4core/src/c4/base64.cpp",
        "ext/c4core/src/c4/blob.hpp",
        "ext/c4core/src/c4/bitmask.hpp",
        "ext/c4core/src/c4/charconv.hpp",
        "ext/c4core/src/c4/c4_pop.hpp",
        "ext/c4core/src/c4/c4_push.hpp",
        "ext/c4core/src/c4/char_traits.cpp",
        "ext/c4core/src/c4/char_traits.hpp",
        "ext/c4core/src/c4/common.hpp",
        "ext/c4core/src/c4/compiler.hpp",
        "ext/c4core/src/c4/config.hpp",
        "ext/c4core/src/c4/cpu.hpp",
        "ext/c4core/src/c4/ctor_dtor.hpp",
        "ext/c4core/src/c4/dump.hpp",
        "ext/c4core/src/c4/enum.hpp",
        "ext/c4core/src/c4/error.cpp",
        "ext/c4core/src/c4/error.hpp",
        "ext/c4core/src/c4/export.hpp",
        "ext/c4core/src/c4/format.hpp",
        "ext/c4core/src/c4/format.cpp",
        "ext/c4core/src/c4/hash.hpp",
        "ext/c4core/src/c4/language.hpp",
        "ext/c4core/src/c4/language.cpp",
        "ext/c4core/src/c4/memory_resource.cpp",
        "ext/c4core/src/c4/memory_resource.hpp",
        "ext/c4core/src/c4/memory_util.cpp",
        "ext/c4core/src/c4/memory_util.hpp",
        "ext/c4core/src/c4/platform.hpp",
        "ext/c4core/src/c4/preprocessor.hpp",
        "ext/c4core/src/c4/restrict.hpp",
        "ext/c4core/src/c4/span.hpp",
        "ext/c4core/src/c4/std/std.hpp",
        "ext/c4core/src/c4/std/std_fwd.hpp",
        "ext/c4core/src/c4/std/string.hpp",
        "ext/c4core/src/c4/std/string_fwd.hpp",
        "ext/c4core/src/c4/std/tuple.hpp",
        "ext/c4core/src/c4/std/vector.hpp",
        "ext/c4core/src/c4/std/vector_fwd.hpp",
        "ext/c4core/src/c4/substr.hpp",
        "ext/c4core/src/c4/substr_fwd.hpp",
        "ext/c4core/src/c4/szconv.hpp",
        "ext/c4core/src/c4/type_name.hpp",
        "ext/c4core/src/c4/types.hpp",
        "ext/c4core/src/c4/unrestrict.hpp",
        "ext/c4core/src/c4/utf.hpp",
        "ext/c4core/src/c4/utf.cpp",
        "ext/c4core/src/c4/windows.hpp",
        "ext/c4core/src/c4/windows_pop.hpp",
        "ext/c4core/src/c4/windows_push.hpp",
        "ext/c4core/src/c4/c4core.natvis",
        "ext/c4core/src/c4/ext/debugbreak/debugbreak.h",
        "ext/c4core/src/c4/ext/rng/rng.hpp",
        "ext/c4core/src/c4/ext/sg14/inplace_function.h",
	}

	includedirs
	{
        "src",
		"ext/c4core/src",
	}


	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "off"

	filter "system:linux"
		pic "On"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "off"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
