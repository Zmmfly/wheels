target("xhash")
    set_kind("static")
    add_files("src/*.c")
    add_includedirs("include", {public=true})

includes("test")