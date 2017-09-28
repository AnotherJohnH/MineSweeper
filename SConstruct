
source  = ['Source/mines.cpp']
binary  = 'mines'
app     = 'mines'
version = '0.0.0'

# Get a build environment
env = SConscript('Platform/Source/build.scons', ['source', 'app', 'version'])

# Project specific build config
env.Append(CCFLAGS = ['-g', '-O3'])

# Builders
exe = env.Program(binary, source)

env.Tar(app+'_'+env['target']+'_'+env['machine']+'_'+version+'.tgz',
        [exe, env['platform_files'], 'LICENSE'])

