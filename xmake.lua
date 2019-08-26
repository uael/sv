set_project("libsv")
set_version("0.0.1")

set_warnings("all", "error")

if is_plat("macosx") or is_plat("iphoneos") then
    add_cflags("-Wno-nullability-completeness")
    add_cxxflags("-Wno-nullability-completeness")
end

add_rules("mode.debug", "mode.release")

target("sv")
    set_default(true)
    set_languages("c99")
    set_kind("$(kind)")
    add_headerfiles("include/(*.h)")
    add_includedirs("include", {public = true})
    add_files("src/*.c")

includes("test")
