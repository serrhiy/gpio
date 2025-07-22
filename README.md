### How to start
1. Create ```CMakeUserPresets.json```:
```json
{
  "version": 2,
  "configurePresets": [
    {
      "name": "default",
      "inherits": "vcpkg",
      "environment": {
        "VCPKG_ROOT": "<path to vcpkg>"
      }
    }
  ]
}
```
2. Install dependencies for libgpiod:
`sudo apt install -y autotools autoconf-archive libtool pkg-config`

3. Build:
`cmake --preset=default`
`cmake --build build`
