========================================================================================================================
New Bluetooth 5 features in Internet of Things Applications - Master's Degree Thesis
========================================================================================================================

Introduction
--------------------------------------------------------------------------------

The aim of this README is to provide a project map to serve navigation purposes.

This repo contains all files related with my master's thesis - *New Bluetooth 5 features in Internet of Things Applications*. The thesis deals with the following aspects:

* Introduces to the most popular short-range wireless technologies: Bluetooth (especially LE and Mesh), ZigBee and Thread
* Designs and develops a full stack of tools to perform *Packet Error Rate* experiment
* Provides methodology, tools and hypothesis of two experiments: power consumption, *Packet Error Rate*
* Analyzes and presents empirical data
* Draws conclusions

**Note:** The true master's text is written in Polish language and will very likely not be translated into English (or any other language). Source code is kept in English, though. Feel free to play around.

Structure
--------------------------------------------------------------------------------

The project consists several directories, each one contains individual parts of my master's thesis.

* `3d_models <https://github.com/krkruk/stm32wb_mesh_packet_error_rate/tree/master/3d_models>`_ - contains 3D case models for P-NUCLEO-WB55 dev board
* `field_experiment <https://github.com/krkruk/stm32wb_mesh_packet_error_rate/tree/master/field_experiment>`_ - contains raw data gathered throughout the experiment sessions and processing scripts written in python (scipy+seaborn)
* `software_ble <https://github.com/krkruk/stm32wb_mesh_packet_error_rate/tree/master/software_ble>`_ - STM32WB55-based software to support *Packet Error Rate* experiment. Note: The software is based on ST's stack. Please follow ST's licensing if in doubt.
* `software_desktop/DesktopApplication <https://github.com/krkruk/stm32wb_mesh_packet_error_rate/tree/master/software_desktop/DesktopApplication>`_ - desktop application source code written in Qt5 supporting ST's Mesh Serial Gateway interface (AT-alike commands).
* `thesis <https://github.com/krkruk/stm32wb_mesh_packet_error_rate/tree/master/thesis>`_ - LaTeX-based thesis text written in Polish + images. It includes the final defense presentation.

Licensing
--------------------------------------------------------------------------------

* 3D Models - created using Solid Edge Community Edition. If you decide to use the models, please make sure you follow Siemens' licesning schema
* Software_ble - ST software based on `STM32CubeWB <https://github.com/STMicroelectronics/STM32CubeWB>`_. The firmware is mostly distributed under libre licenses (Apache-2.0, BSD-3-Clause). 
* software_desktop - refer to Qt's Open Software Licensing
* thesis - the thesis uses a template originally created by SP5LMA `EE-dyplom <https://github.com/SP5LMA/EE-dyplom>`_. Big thanks for sharing the template!

Anything else can be distributed under CC BY 4.0 license.
