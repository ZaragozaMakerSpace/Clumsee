# Clumsee
<b>Clumsee</b> is a robot that has a color recognition sensor in order to extend games based on election on a color board table.
He can read <b>RGB colours</b> but also distinguish between:

<ul>
	<li>RED</li>
	<li>GREEN</li>
	<li>BLUE</li>
	<li>YELLOW</li>
	<li>ORANGE</li>
	<li>BLACK</li>
	<li>WHITE</li>
</ul>

<h1>Clumsee</h1>
<img src="src/ClumseeRobot.png" alt="">

Libraries used on this model
<ul>
	<li><a href="libraries">TCS3200</a></li>
	<li><a href="libraries">IMU_MPU9250</a></li>
	<li><a href="libraries">Madgwick</a></li>
</ul>

There are two different models one <b>Color Sensor</b> or with with two color sensors for other applications
<ul>
	<li>One Color Sensor</li>
	<li>Two Color Sensors</li>
</ul>


<h2>IMU Visualization and control</h2>
It also can read <b>Roll, Pitch, Yaw</b> Orientation for different purpose.

<h2>Remote Control</h2>
This model can be controlled by an Android App or other devices connected with <b>Bluetooth HC-05</b>.

<h3>App Inventor Programm</h3>

<h3>Esplora Remote Control</h3>
This model can be controlled with an <b>Arduino Esplora</b> and a <b>TFT Screen ST7735</b> to extend more games with <b>Clumsee</b> made with Arduino.

Libraries used on this model
<ul>
	<li><a href="libraries">Adafruit_GFX</a></li>
	<li><a href="libraries">Adafruit_ST7735</a></li>
</ul>

It´s neccesary configure some aspects inside our HC-05 Bluetooth and connect properly with <b>Arduino Esplora</b> in the following <a href="http://blascarr.com/esplora-hc-05-bluetooth-remote-control/">webpage</a>.

Documentation about <b>Clumsee</b> can be readed in <a href="http://blascarr.com/courses/clumsee/"> Blascarr Webpage</a>
