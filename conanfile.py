from conan import ConanFile


class InterM2(ConanFile):
    generators = "CMakeDeps", "CMakeToolchain"
    settings = "os", "compiler", "build_type", "arch"

    def requirements(self):
        self.requires("mongo-cxx-driver/3.8.0")
        self.requires("simdjson/3.2.2")
        self.requires("boost/1.82.0", override=True)
