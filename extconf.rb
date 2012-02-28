require 'mkmf'

# for MacPorts
dir_config('portaudio', '/opt/local/include', '/opt/local/lib')

have_library('portaudio')
create_makefile('roscil')

