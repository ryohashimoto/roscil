# Roscil

* Author: Ryo Hashimoto
* Copyright: Copyright (c) 2012 Ryo Hashimoto
* License: MIT License

This package contains simple Ruby sound generator using PortAudio.
PortAudio is a cross-platform interface to audio hardwares.
http://www.portaudio.com

You can generate sounds by following codes.

    require 'roscil'
    
    Roscil.sin(440) # 440Hz sine wave
    Roscil.saw(220) # 220Hz sawtooth wave
    Roscil.tri(110) # 110Hz triangle wave
    Roscil.pulse(880) # 880Hz pulse wave

We tested this package on following platforms.

* Mac OS X 10.6.8, Xcode 3.2.6 and MacPorts 2.0.3
* Ubuntu 10.04 LTS (32bit) and PortAudio v19 installed from the sounce (pa_stable_v19_20111121)

In Mac systems with MacPorts, you can install PortAudio library by

    sudo port install portaudio

In deb based Linux systems

    sudo apt-get install libportaudio-dev

If you use this package on Windows or other systems, please modify extconf.rb.

## Requirements

* PortAudio, v19 or higher
* Ruby, version 1.9.3 or higher

## Building gem

You can build gem package by following.
    $ cd roscil
    $ gem build roscil.gemspec
    $ gem install roscil-0.0.1.gem
