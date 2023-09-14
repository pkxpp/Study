import taichi as ti

ti.init(arch=ti.gpu)

# Constants
dim = 2
num_particles = 10000
dx = 1 / 64
inv_dx = 1 / dx
dt = 1e-4
p_vol = (dx * 0.5)**2
p_rho = 1
p_mass = p_vol * p_rho
E = 400
v_max = 10
v_min = -10

# Grids
grid_size = 512
grid_s = 1 / grid_size
cell_size = dx
grid_particle_num = ti.field(ti.i32)
grid_v = ti.Vector(dim, dt=ti.f32)
grid_m = ti.field(ti.f32)
grid_c = ti.field(ti.f32)

# Particles
x = ti.Vector(dim, dt=ti.f32, shape=num_particles)
v = ti.Vector(dim, dt=ti.f32, shape=num_particles)
f = ti.Vector(dim, dt=ti.f32, shape=num_particles)
rho = ti.field(ti.f32, shape=num_particles)
m = ti.field(ti.f32, shape=num_particles)
c = ti.field(ti.f32, shape=num_particles)

# Kernel
def W(r, h):
    coeff = 315 / (64 * 3.1415926 * h**9)
    q = r / h
    if q < 1:
        return coeff * (h**2 - r**2)**3
    else:
        return 0

# Neighbors
@ti.kernel
def find_neighbors():
    for i in range(num_particles):
        grid_particle_num[x[i] * inv_dx] += 1
    for i in range(num_particles):
        neighbors = []
        pos = x[i] * inv_dx
        for offset in ti.static(ti.grouped(ti.ndrange((-1, 2), (-1, 2)))):
            cell_pos = int(pos + offset)
            if all(cell_pos >= 0) and all(cell_pos < grid_size):
                begin = grid_particle_num[cell_pos]
                end = grid_particle_num[cell_pos + 1]
                for j in range(begin, end):
                    if i != j:
                        neighbors.append(j)
        grid_particle_num[pos] = 0
        for j in neighbors:
            grid_particle_num[pos] += 1
        grid_particle_num[pos + 1