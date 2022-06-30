-- add_requires("gtest")

target("xhash_test")
    set_default(false)
    set_kind("binary")
    set_targetdir("dist")
    add_deps("xhash")
    add_ldflags("-static")
    -- add_packages("gtest", {syslinks = {"pthread"}})
    -- add_links("pthread")
    add_files("src/**.cc")
    if is_plat("windows") then
        add_syslinks("wsock32")
    end
