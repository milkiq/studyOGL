add_requires("stb", "glfw", "opengl", "glad", "eigen")
add_requires("glslang", {configs = {binaryonly = true}})

add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})
add_rules("mode.debug", "mode.release")


-- glsl rule

rule("custom.glsl2spv")
    set_extensions(".vert", ".tesc", ".tese", ".geom", ".comp", ".frag", ".comp", ".mesh", ".task", ".rgen", ".rint", ".rahit", ".rchit", ".rmiss", ".rcall", ".glsl")
    on_load(function (target)
        local is_bin2c = target:extraconf("rules", "custom.glsl2spv", "bin2c")
        if is_bin2c then
            local headerdir = path.join(target:autogendir(), "rules", "utils", "glsl2spv")
            if not os.isdir(headerdir) then
                os.mkdir(headerdir)
            end
            target:add("includedirs", headerdir)
        end
    end)
    before_buildcmd_file(function (target, batchcmds, sourcefile_glsl, opt)
        import("lib.detect.find_tool")

        -- get glslangValidator
        local glslc
        local glslangValidator = find_tool("glslangValidator")
        if not glslangValidator then
            glslc = find_tool("glslc")
        end
        assert(glslangValidator or glslc, "glslangValidator or glslc not found!")

        -- glsl to spv
        local targetenv = target:extraconf("rules", "custom.glsl2spv", "targetenv") or "vulkan1.0"
        local client = target:extraconf("rules", "custom.glsl2spv", "client") or "vulkan100"
        local outputdir = target:extraconf("rules", "custom.glsl2spv", "outputdir") or path.join(target:autogendir(), "rules", "custom", "glsl2spv")
        local spvfilepath = path.join(outputdir, path.filename(sourcefile_glsl) .. ".spv")
        batchcmds:show_progress(opt.progress, "${color.build.object}generating.glsl2spv %s", sourcefile_glsl)
        batchcmds:mkdir(outputdir)
        if glslangValidator then
            batchcmds:vrunv(glslangValidator.program, {"--target-env", targetenv, "--client", client, "-o", path(spvfilepath), path(sourcefile_glsl)})
        else
            batchcmds:vrunv(glslc.program, {"--target-env", targetenv, "-o", path(spvfilepath), path(sourcefile_glsl)})
        end

        -- do bin2c
        local outputfile = spvfilepath
        local is_bin2c = target:extraconf("rules", "custom.glsl2spv", "bin2c")
        if is_bin2c then
            -- get header file
            local headerdir = outputdir
            local headerfile = path.join(headerdir, path.filename(spvfilepath) .. ".h")
            target:add("includedirs", headerdir)
            outputfile = headerfile

            -- add commands
            local argv = {"lua", "private.utils.bin2c", "--nozeroend", "-i", path(spvfilepath), "-o", path(headerfile)}
            batchcmds:vrunv(os.programfile(), argv, {envs = {XMAKE_SKIP_HISTORY = "y"}})
        end

        -- add deps
        batchcmds:add_depfiles(sourcefile_glsl)
        batchcmds:set_depmtime(os.mtime(outputfile))
        batchcmds:set_depcache(target:dependfile(outputfile))
    end)

--glsl rule end

target("studyOpenGL")
    set_kind("binary")
    add_rules("custom.glsl2spv", {bin2c = true, targetenv = "opengl", client = "opengl100"})
    add_files("src/**.cpp")
    add_files("shaders/*.vert", "shaders/*.frag")
    add_packages("stb", "glslang", "glfw", "opengl", "glad", "eigen")

    after_build_file(function(target, sourcefile, opt)
            if os.exists("./resources") then
                os.cp("./resources",target:targetdir())
            end
        end)

    if is_plat("macosx") then
        after_build_file(function(target,sourcefile,opt)
                if os.exists("./shaders") then
                    os.cp("./shaders",target:targetdir())
                end
            end)
    end

--
-- If you want to known more usage about xmake, please see https://xmake.io
--
-- ## FAQ
--
-- You can enter the project directory firstly before building project.
--
--   $ cd projectdir
--
-- 1. How to build project?
--
--   $ xmake
--
-- 2. How to configure project?
--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
-- 3. Where is the build output directory?
--
--   The default output directory is `./build` and you can configure the output directory.
--
--   $ xmake f -o outputdir
--   $ xmake
--
-- 4. How to run and debug target after building project?
--
--   $ xmake run [targetname]
--   $ xmake run -d [targetname]
--
-- 5. How to install target to the system directory or other output directory?
--
--   $ xmake install
--   $ xmake install -o installdir
--
-- 6. Add some frequently-used compilation flags in xmake.lua
--
-- @code
--    -- add debug and release modes
--    add_rules("mode.debug", "mode.release")
--
--    -- add macro definition
--    add_defines("NDEBUG", "_GNU_SOURCE=1")
--
--    -- set warning all as error
--    set_warnings("all", "error")
--
--    -- set language: c99, c++11
--    set_languages("c99", "c++11")
--
--    -- set optimization: none, faster, fastest, smallest
--    set_optimize("fastest")
--
--    -- add include search directories
--    add_includedirs("/usr/include", "/usr/local/include")
--
--    -- add link libraries and search directories
--    add_links("tbox")
--    add_linkdirs("/usr/local/lib", "/usr/lib")
--
--    -- add system link libraries
--    add_syslinks("z", "pthread")
--
--    -- add compilation and link flags
--    add_cxflags("-stdnolib", "-fno-strict-aliasing")
--    add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})
--
-- @endcode
--

