project('arduino relay-board', 'c', 'cpp',
  default_options : ['buildtype=minsize',
                     'b_lto=true',
                     ],
  )

assert(meson.is_cross_build(), 'Arduino projects can only be built in a cross build environment.')

ardu_src = meson.get_cross_property('ardu_src')

exe = executable('relay-board', ['relay-board.cpp'] + ardu_src,
  link_args : '-lm')

objcopy = find_program('avr-objcopy')
avrdude = find_program('avrdude')

eep = custom_target('relay-board.eep',
  input : exe,
  output : 'relay-board.eep',
  command : [objcopy, '-O', 'ihex', '-j', '.eeprom',
    '--set-section-flags=.eeprom=alloc,load',
    '--no-change-warnings', '--change-section-lma',
    '.eeprom=0', '@INPUT@', '@OUTPUT@',
  ]
)

hex = custom_target('relay-board.hex',
  input : exe,
  output : 'relay-board.hex',
  command : [objcopy, '-O', 'ihex', '-R', '.eeprom',
    '@INPUT@', '@OUTPUT@',
  ],
  depends : eep,
)

run_target('ardup',
  command : [avrdude,
  '-C/usr/share/arduino/hardware/tools/avrdude.conf',
  '-v', '-v', '-v', '-v',
  '-patmega328p', '-carduino', '-P/dev/ttyACM0', '-b115200',
  '-D', '-Uflash:w:@0@:i'.format(hex.full_path())],
  depends : hex,
)
