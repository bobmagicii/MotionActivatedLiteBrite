# Motion Activated Lite-Brite.

This is a hack to modify a modern LED based battery powered Lite-Brite to
have motion sensing. The unit is thin enough that you can hang on the wall
but the button to turn it on is on the back. Additionally, it has a timeout
of 5 minutes. Solution was to just have it automatically turn on every time
I walk into the or move around within it enough. This works by emulating a
button press every time the motion sensor trips.

Most of the hardware I used came from a giant kit that all together cost
as much as my specific mainboard does solo so I am not quite sure how that
works out. Everything about this project is stupid overkill, making this
likely the most expensive Lite-Brite on the planet, at least until some rich
douche gets one gold plated.

I elected to keep the Lite-Brite circuitry isolated from the Arduino with
a relay so that firstly the batteries in the toy last as long as they can
and second because I know enough about electronics to be stupid and not so
much smart. So I let that be, and all the addons are powered by a USB phone
charger.

If using the hardware timer built onto the motion sensor like I am this
code has the ability to put the Arduino into sleep mode to save as much
power as it can, though I've yet to test how long this disaster of
electronical engineering can last on a 9v battery.

# Software Used

* VS Code with Arudino Plugin from Microsoft
	- Requires Arudino IDE or CLI

# Hardware Used

* Mainboard:
	- Arduino Mega 2560

* Components
	- Power MB V2
	- HC-SR501 PIR motion sensor
	- L293D Quadruple Half-H driver chip
	- SRD-05VDC-SL-C relay
	- A red, an amber, and a blue LED
	- three 5K1 resistors one for each LED

# Compile and Upload

The Arduino for VS Code kind of sucks in terms of being good for shared
development. For example you will need to edit the filepaths in
`c_cpp_properties.json` to have any hope unless your Windows username
just happens to be `bob` like mine is. For some reason nobody over there
though to use placeholders that can then later make use of the magic crap
they already have that detected all that when I first set up the project.

But then you can use the Verify and Upload buttons in the top right of
the VS Code UI added by the plugin and that is about all there is to it.
Make sure to select the mainboard you're using on the bottom of the window.

# Known Issues

* VS Code complains about all the sleep function stuff even though it was
  totally able to find `sleep.h` and even make it clickable to open it.
  Nevertheless it manages to compile and upload cleanly.

# Schematic

[todo: pictures n stuff]

# Modding the Lite-Brite

[todo: photos n stuff]

The TL;DR is on the mainboard inside the toy there is a button that uses
one of those silicone squishy jobbers to act as both the circuit finisher
and spring to push the button back up. On both sides of that there are
exposed areas of the circuit traces that I assume were used to automate testing. I soldered two wires, one on to each of those pads and routed them
out the back of the toy through a hole in the case already there. That is
the extent of the hacking. Those two wires go onto the COM and NO
connections of the relay, the relay clicks, button is pretend pushed.

To actually do the soldering I gently pulled the rubber jobber out of the
board so that it wouldn't get accidentally by the hot soldering iron. Then gently squished it back on after.

