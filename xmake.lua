add_rules("mode.debug", "mode.release")
set_languages("c++latest")

target("tru-lang")
    set_kind("binary")
    add_files("src/*.cpp", "src/*.cppm")
