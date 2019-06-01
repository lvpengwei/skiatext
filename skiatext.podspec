Pod::Spec.new do |s|
  s.name     = 'skiatext'
  s.version  = '0.0.1'
  s.ios.deployment_target   = '8.0'
  s.summary  = 'DESC'
  s.homepage = 'http://www.lvpengwei.com'
  s.author   = { 'lvpengwei' => 'pengwei.lv@gmail.com' }
  s.requires_arc = true 
  s.source   = {
    :git => ''
  }

  s.source_files = 'include/**/*.{h,cpp,mm}','src/rendering/**/*.{h,cpp,mm}','vendor/skia/include/**/*.{h,cpp,mm}'
  s.xcconfig = { "HEADER_SEARCH_PATHS" => "${PODS_ROOT}/#{s.name}/vendor/skia/include/** ${PODS_ROOT}/../../../../vendor/skia/include/**"}
  s.compiler_flags = '-Wno-documentation','-Wno-unused-variable'

  s.ios.public_header_files = 'src/platform/ios/*.h'
  s.ios.source_files = 'src/platform/ios/**/*.{h,cpp,mm,m}'
  s.ios.vendored_libraries  = 'vendor/skia/ios/libskia.a'
  s.ios.frameworks   = ['UIKit', 'CoreFoundation', 'QuartzCore', 'CoreGraphics', 'CoreText', 'OpenGLES']

end
