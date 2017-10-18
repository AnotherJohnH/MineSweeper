
source  = ['Source/mines.cpp']
binary  = 'mines'
app     = 'mines'
version = '0.0.1'

# Get a build environment
env,lib = SConscript('Platform/build.scons', ['app', 'version'])

# Project specific build config
env.Append(CCFLAGS = ['-O3'])

# Builders
exe = env.Program(binary, source)
Depends(exe, lib)

env.Tar(app+'_'+env['target']+'_'+env['machine']+'_'+version+'.tgz',
        [exe, env['platform_files'], 'LICENSE'])

