add_rules("mode.debug", "mode.release")
set_languages("c++latest")

target("tru-lang")
    set_kind("binary")
    add_files("src/*.cpp", "src/*.cppm")
	set_policy("build.c++.modules.std", false)
	add_cxxflags("-stdlib=libc++", { force = true })
	add_ldflags("-stdlib=libc++", { force = true })
