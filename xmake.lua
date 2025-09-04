set_project("libsv")
set_version("0.0.1")
set_xmakever("3.0.1")

add_rules("mode.debug", "mode.release")
set_warnings("all", "error")

if is_plat("macosx", "iphoneos") then
    add_cflags("-Wno-nullability-completeness")
    add_cxxflags("-Wno-nullability-completeness")
end

target("sv")
    set_default(true)
    set_languages("c99")
    set_kind("$(kind)")
    add_headerfiles("include/(*.h)")
    add_includedirs("include", {public = true})
    add_files("src/*.c")

includes("test")
