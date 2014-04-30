Gem::Specification.new do |spec|
  spec.name = "roscil"
  spec.version = "1.0.0"
  spec.platform = Gem::Platform::RUBY
  spec.summary = "Sound generator (Using PortAudio) for Ruby"
  spec.files = ["README.md"] + Dir.glob('lib/**/*.rb') + Dir.glob('ext/**/*.{c,h,rb}')
  spec.extensions = ['ext/roscil/extconf.rb']
  spec.require_path = "lib"
  spec.has_rdoc = false
  spec.author = "Ryo Hashimoto"
  spec.email = "ryohashimoto@gmail.com"
  spec.rubyforge_project = "roscil"
  spec.homepage = "https://github.com/ryohashimoto/roscil"
  spec.description = "Sound generator (Using PortAudio) for Ruby"
end
