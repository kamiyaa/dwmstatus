# update config.h
run_command('cp',
        ['-u', 'config.def.h', 'config.h']
        )

dep_xcb = dependency('xcb')

helpers = ['dwmstatus.c']

executable('dwmstatus_std',
	'dwmstatus_std.c', helpers,
	install: true
	)

executable('dwmstatus_xcb',
	'dwmstatus_xcb.c', helpers,
	dependencies : dep_xcb,
	install: true
	)

