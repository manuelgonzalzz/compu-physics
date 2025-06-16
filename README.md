# 🌌 Computational Physics – Winter Semester 2023/2024

This repository contains assignments and a voluntary project completed for the *Computational Physics* course at **Universidad de Granada** during the Winter Semester 2023/2024.

## 📁 Repository Structure

```
CompuPhysics-gitcompu/
├── p1+voluntario1/
│   ├── sistema_solar.cpp               # Planetary motion simulation (C++)
│   ├── energia.txt                     # Total energy output
│   ├── periodos.txt                    # Orbital periods
│   ├── radius.txt                      # Planetary distances
│   └── voluntario/
│       ├── planetesimales.cpp          # Planetesimal collision simulation (C++)
│       ├── animacion_planetesimales.py # Python animation of results
│       ├── planetafinal.png            # Final planetary system visualization
│       ├── *.txt                       # Simulation input/output data
│       └── *.png                       # Generated plots
```

## 🔬 Topics Covered

Numerical integration techniques: Euler method and Verlet method. Simulation of solar system dynamics. Modeling gravitational interactions and perturbations. Collision dynamics of planetesimals. Analysis of energy conservation and orbital parameters. Visualization and animation using Python (`matplotlib`).

## 🚀 How to Run

### 🔧 C++ Simulation

To compile and run the planetary system simulation:

```bash
g++ sistema_solar.cpp -o sistema_solar
./sistema_solar
```

### 🎞️ Python Animation (Voluntary Project)

To visualize the planetesimal collision simulation:

```bash
python3 animacion_planetesimales.py
```

#### 📦 Required Library

Make sure `matplotlib` is installed:

```bash
pip install matplotlib
```

## 📌 Notes

- Output files, code comments, and variable names are primarily in **Spanish**.
- The `voluntario` folder contains an **advanced project** focused on collision modeling and final system visualization.
- These simulations are academic and simplified representations of physical systems, built for educational purposes.


