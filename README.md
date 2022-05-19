# USB Audio Interface

This repo (will) contain firmware, schematics, and instructions for creating a functional audio interface usable cross-platform over USB. The current design supports one balanced XLR input with 80dB gain, and a 1/4 inch input with 40dB gain.

This design was created for the course CSE 145: Embedded Systems Design Project at UCSD in Spring 2022.

![Systems Diagram](docs/sys_diagram.png)

## Electrical

Inside the [audio_interface](audio_interface) subfolder, there is a KiCAD project with both the schematic and layout for the custom PCB. For easier viewing, the [schematic](docs/audio_interface.pdf) and [layout](docs/audio_layout.pdf) are exported as pdfs as well. Note that many of the component choices were made due to the specific availability during the silicon shortage in Spring 2022. Other substitutes may be cheaper or easier to obtain in the future. To see the total list of necessary components, as well as what is currently in stock, check out the [Octopart BOM](https://octopart.com/bom-tool/Pp2dbWVm).

There is a [test log](https://docs.google.com/document/d/14NlvdZo9BEKJ16lZTaRw5GM3TA98BqlqD3HZuE-fhHs/edit?usp=sharing) which documents all the of the properties of the design, comparing the measured vs expected values. In addition, a listing at the bottom functions as a notebook to record which rework was performed on which day, and other observations.

## Audio Quality
There is a short recording demonstrating both inputs in the [docs](docs) directory. In addition, here are some frequency plots showing the results from recording a generated sine wave on both inputs. As you can see there is currently some noise and distortion that is being worked out.

## Contributors

