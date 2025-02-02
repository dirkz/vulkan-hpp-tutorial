# Vulkan Tutorial, C++ style

Let's do the official [Vulkan Tutorial](https://docs.vulkan.org/tutorial/latest/00_Introduction.html),
but with pure [Vulkan-Hpp](https://github.com/KhronosGroup/Vulkan-Hpp).

I think it will be a lot of fun, and may be even yield code that can be used as a base
for a decent vertex pipeline for other projects.

You'll need the [Vulkan SDK](https://vulkan.lunarg.com/sdk/home), or you
[build it yourself](https://github.com/dirkz/vulkan-sdk-builder).

Other dependencies are "vendored in", like GLM and GLfW. In this case, that means
they are integrated as git submodules.

Please note that I tend to prefer ssh repository URL wherever possible.

## How to check out

Either from the start:

```
git clone --recurse-submodules git@github.com:dirkz/vk-hpp-tutorial.git
```

Or after a "normal" clone:

```
git submodule update --init --recursive
```