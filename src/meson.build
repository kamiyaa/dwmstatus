# update config.h
run_command('cp',
        ['-u', 'config.def.h', 'config.h']
        )

config_h = custom_target('config.h',
		input : ['config.def.h'],
		output : ['config.h'],
		command : ['cp', '@INPUT0@',
		'@OUTPUT0@'])

dep_alsa = dependency('alsa')
dep_xcb = dependency('xcb')
dep_xcb_util = dependency('xcb-util')

dwmstatus = declare_dependency(
		sources: 'dwmstatus.c',
		dependencies: dep_alsa
		)

executable('dwmstatus_std',
	'dwmstatus_std.c',
	dependencies: dwmstatus,
	install: true
	)

executable('dwmstatus_xcb',
	'dwmstatus_xcb.c',
	dependencies : [dwmstatus, dep_xcb, dep_xcb_util],
	install: true
	)

