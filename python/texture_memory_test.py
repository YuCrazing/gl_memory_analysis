import os
import numpy as np
import psutil

from pyvirtualdisplay import Display
import pyrender
import trimesh

def memory_usage_psutil():
    """Return the memory usage in MB"""
    process = psutil.Process(os.getpid())
    mem = process.memory_info().rss / (1024 ** 2)
    return mem

def main():
    # Start the virtual display
    display = Display(visible=0, size=(800, 600))
    display.start()
    try:
        # Define texture size (adjust size to test memory footprint)
        width, height = 4096, 4096  # Large texture for significant memory usage

        # Generate random texture data (RGBA channels)
        texture_data = np.random.randint(0, 256, (height, width, 4), dtype=np.uint8)
        print(f"Texture data memory: {texture_data.nbytes / (1024 ** 2):.2f} MB")
        
        # Measure memory before texture creation
        mem_before = memory_usage_psutil()
        print(f"Memory before pyrender.Texture: {mem_before:.2f} MB")
        
        # Create a pyrender.Texture
        texture = pyrender.Texture(source=texture_data, source_channels='RGBA')

        # Measure memory after texture creation
        mem_after = memory_usage_psutil()
        print(f"Memory after pyrender.Texture: {mem_after:.2f} MB")
        print(f"Memory difference: {mem_after - mem_before:.2f} MB")

        # # Optionally, add the texture to a material and render a scene
        # # Create a PBR material with the texture
        # material = pyrender.MetallicRoughnessMaterial(
        #     baseColorTexture=texture,
        #     metallicFactor=0.0,
        #     roughnessFactor=0.5
        # )

        # # Create a mesh with the material
        # trimesh_mesh = trimesh.creation.box()
        # mesh = pyrender.Mesh.from_trimesh(trimesh_mesh, material=material)

        # # Create a scene and add the mesh
        # scene = pyrender.Scene()
        # scene.add(mesh)

        # # Set up the camera
        # camera = pyrender.PerspectiveCamera(yfov=np.pi / 3.0)
        # camera_pose = np.array([
        #     [1.0, 0.0, 0.0, 0.0],
        #     [0.0, 1.0, 0.0, -1.0],
        #     [0.0, 0.0, 1.0, 2.5],
        #     [0.0, 0.0, 0.0, 1.0]
        # ])
        # scene.add(camera, pose=camera_pose)

        # # Add a light source
        # light = pyrender.DirectionalLight(color=np.ones(3), intensity=3.0)
        # scene.add(light, pose=camera_pose)

        # # Create an offscreen renderer
        # renderer = pyrender.OffscreenRenderer(viewport_width=800, viewport_height=600)
        # color, depth = renderer.render(scene)

        # # Clean up
        # renderer.delete()
    finally:
        # Stop the virtual display
        display.stop()

if __name__ == '__main__':
    main()