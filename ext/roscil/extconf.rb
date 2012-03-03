require 'mkmf'

# for MacPorts and PortAudio installed from source.
dir_config('portaudio', '/opt/local/include', '/opt/local/lib')

have_library('portaudio')
create_makefile('roscil/roscil')
