import os
import subprocess as sp
import numpy as np
import skimage
from skimage import io
import io
import json

def readline_ascii(fstream):
    return fstream.readline().decode(encoding="ascii")


def read_ppm_image(fstream):
    magic_line = readline_ascii(fstream)
    assert magic_line[0] == "P"
    assert magic_line[1] == "6"

    line = readline_ascii(fstream)
    while line[0] == "#":
        line = readline_ascii(fstream)

    num_columns_line = line
    num_columns = int(num_columns_line)
    num_rows_line = readline_ascii(fstream)
    num_rows = int(num_rows_line)

    max_color_line = readline_ascii(fstream)
    max_color = int(max_color_line)

    assert max_color == 255
    size_bytes = num_columns*num_rows*3

    raw = np.frombuffer(fstream.read(size_bytes), dtype=np.uint8)
    img = raw.reshape((num_rows, num_columns, 3))
    return img


server_path = "./camsrv"
scenery_path = "merlict_c89/tests/resources/sceneries/optics_focussing_mirror.tar"
outdir = 'fly'

def image_sensor_distance(focal_length, object_distance):
    # 1/f = 1/b + 1/g
    b = (1.0) / (1/focal_length - 1/object_distance)
    return b


CAMERA = {
    "pos": [0, -8.9e-1, 2.62e-1],
    "rot": {"repr": "tait_bryan", "xyz_deg": [110,0,0]},
    "focal_length": 5e-3,
    "aperture_radius": 1.25e-3,
    "image_sensor_distance": image_sensor_distance(
        focal_length=5e-3,
        object_distance=0.85,
    ),
    "image_sensor_width_x": 3.6e-3,
    "image_sensor_width_y": 2.025e-3,
}

IMAGE = {
    "num_pixel_x": 128,
    "num_pixel_y": 72,
}

TRACER = {
    "num_trails_global_light_source": 1,
    "have_atmosphere": 1,
    "background_color": [128, 128, 128],
    "atmosphere": {
        "sunLatitude": 0,
        "sunHourAngle": 12.0,
        "sunDistance": 1.5e11,
        "sunRadius": 7e8,
        "earthRadius": 6360e3,
        "atmosphereRadius": 6360e3 + 60e3,
        "Height_Rayleigh": 7994.0,
        "Height_Mie": 1200.0,
        "numSamples": 16,
        "numSamplesLight": 8,
        "beta_Rayleigh": [3.8e-6, 13.5e-6, 33.1e-6],
        "beta_Mie": [41e-6, 41e-6, 41e-6],
        "power": 3e3,
        "altitude": 2300.0,
    },
}

EXAMPLE = {
    "random_seed": 1,
    "camera": CAMERA,
    "tracer": TRACER,
    "image": IMAGE,
}

def communicate_control(server, control):
    jsonl_str = json.dumps(control, indent=None) + "\n"
    jsonl_bytes = str.encode(jsonl_str, encoding="ascii")

    server.stdin.write(jsonl_bytes)
    server.stdin.flush()

    response_str = readline_ascii(server.stdout)
    response = json.loads(response_str)
    assert response["status"] == 1, "Server did not understand command."


def request_image(server, control):
    communicate_control(server=server, control=control)
    return read_ppm_image(fstream=server.stdout)


class RenderServer:
    def __init__(self, server_path, scenery_tar_bytes):
        self.server = sp.Popen(server_path, stdin=sp.PIPE, stdout=sp.PIPE)
        self.server.stdin.write(scenery_tar_bytes)
        self.server.stdin.write(b"\n")

    def render_image(self, control):
        return request_image(server=self.server, control=control)

    def save_image(self, control, path):
        image = self.render_image(control=control)
        skimage.io.imsave(path, image)


with open(scenery_path, "rb") as f:
    scenery_tar_bytes = f.read()

server = RenderServer(
    server_path=server_path,
    scenery_tar_bytes=scenery_tar_bytes
)

os.makedirs(outdir, exist_ok=True)
for i, x in enumerate(np.linspace(-0.5, 0.5, 30)):
    print("image [{:d}]".format(i))

    EXAMPLE["random_seed"] = int(i)
    EXAMPLE["camera"]["pos"][0] = x
    EXAMPLE["tracer"]["atmosphere"]["sunLatitude"] = np.deg2rad(5)
    EXAMPLE["tracer"]["atmosphere"]["sunHourAngle"] = (x*2) + 12

    server.save_image(
        control=EXAMPLE,
        path=os.path.join(outdir, '{:06d}.png'.format(i)),
    )
