# Simple 2D planetary motion simulator using point mass representations and Newtonian gravity. 

## Instructions

1. Create a CSV file specifying the orbital bodies as per the example CSV. Each entry in the CSV file follows the following format:
`name of body, mass, initial X position, initial Y position, initial X velocity, initial Y velocity,`
2. Rename the example CSV file to `cot.csv` and copy to the environment of the executable.
3. Run the executable. 

## TODO features to add

### Camera
- Camera control with pan and zoom controls
- Selectable snap to body with auto-zoom to fit body and certain portion of radius
- Show arrows to off-screen bodies
- Show text label on bodies
- Handle window resizing

### Configuration
- External configuration (cot.cfg) file with compiled defaults

### Persist
- Amend persistence line to be distance based not frame based or with option to choose

### Physics
- Collision detection
- Decouple mass and radius relationship with density-based gravity

### Simulation
- Option to simulate ahead of real-time at processor limits
- Playback of previous simulations

### Telemetry
- Transfer current telemetry readout from log file to dat file dump
- Live telemetry readout (table of bodies with XY positions and velocities)
- CPU, GPU, RAM usage metrics

