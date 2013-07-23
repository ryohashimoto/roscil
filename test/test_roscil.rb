lib = File.expand_path('../../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)

require 'test/unit'
require 'roscil'

class Roscil::TestCase < Test::Unit::TestCase

  def test_sin
    Roscil.sin(440)
  end

  def test_tri
    Roscil.tri(220)
  end

  def test_saw
    Roscil.saw(110)
  end

  def test_pulse
    Roscil.pulse(880)    
  end
end
