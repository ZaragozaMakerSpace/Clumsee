/*
  Design and created by Blascarr
  TCS3200
  Name    : Blascarr
  Description: TCS3200.h
  version : 1.0
	TCS3200 is a library for color detection with TCS3200 module.
	The main objective is improve the performance detection of differente range colors
	for simple robotic applications.
	Compute time event intervals to read input signal for non-blocking systems.
	Compute buffer based on mean values and filtering errors.
	Mapping in different range values for colors.
	EEPROM memory for save some calibration data and color list.
	Assign interruption pint to the INPUT signal to calculate samples. (WIP)
  
  This library is a modification of MD_TCS230 created by Marco Colli
    https://github.com/MajicDesigns/MD_TCS230
  	
  	Blascarr invests time and resources providing this open source code like some other libraries, please
  	respect the job and support open-source software.
    
    Written by Adrian for Blascarr
*/
	  
#ifndef _TCS3200_H
#define _TCS3200_H

	#if defined(ARDUINO) && ARDUINO >= 100
		#include "Arduino.h"
	#else
		#include "WProgram.h"
	#endif

    #include <EEPROM.h>

    //BPS Communication
	#ifndef BPS
		#define  BPS 9600
	#endif

    #ifndef DEBUG_RX
    	#define  DEBUG_RX 2
    #endif
    #ifndef DEBUG_TX
		#define  DEBUG_TX 3
    #endif

	//Pattern for DEBUG detection
	#ifndef DEBUG_STARTCMD 
	  #define  DEBUG_STARTCMD "{" 
	#endif
	#ifndef DEBUG_ENDCMD 
	  #define  DEBUG_ENDCMD "}"
	#endif
	#ifndef DEBUG_SEPCM 
	  #define  DEBUG_SEPCMD ","
	#endif

	//Pattern for CALIBRATION detection
	#ifndef DEBUGCAL_STARTCMD 
	  #define  DEBUGCAL_STARTCMD "<"
	#endif
	#ifndef DEBUGCAL_ENDCMD
	  #define  DEBUGCAL_ENDCMD ">"
	#endif
	#ifndef DEBUGCAL_SEPCMD
	  #define  DEBUGCAL_SEPCMD "|"
	#endif

	/*SENDCOLOR
	  0 - No Serial for DUMPCOLOR
	  1 - Serial
	*/
	#ifndef SENDCOLOR
	  #define  SENDCOLOR 1
	#endif

	//Pattern for COLOR detection
	#ifndef COLOR_STARTCMD 
	  #define  COLOR_STARTCMD "#"
	#endif
	#ifndef COLOR_ENDCMD 
	  #define  COLOR_ENDCMD "*"
	#endif
	#ifndef COLOR_SEPCMD
	  #define  COLOR_SEPCMD "|"
	#endif

	//#define SERIALDEBUG Serial
	#ifdef SERIAL_DEBUG
		#define COMM_DEBUG 0
	#else

	#endif

	#ifdef MEGA_DEBUG 
		#if MEGA_DEBUG == 0 
			#define COMM_DEBUG 0
		#elif MEGA_DEBUG == 1 
			#define COMM_DEBUG 1
		#elif MEGA_DEBUG == 2 
			#define COMM_DEBUG 2
		#elif MEGA_DEBUG == 3 
			#define COMM_DEBUG 3
	  	#else
	  		#define COMM_DEBUG 0
		#endif
	#endif

	#ifdef BT_DEBUG
		#include <SoftwareSerial.h>
		SoftwareSerial BT(DEBUG_RX, DEBUG_TX);
		#define COMM_DEBUG 4
	#endif

	
	#if COMM_DEBUG == 0
		#define SERIALDEBUG Serial
		#define MODE "Serial HS MODE"
	#elif COMM_DEBUG == 1
		#define SERIALDEBUG Serial1
		#define MODE "Serial1 HS MODE"
	#elif COMM_DEBUG == 2
		#define SERIALDEBUG Serial2
		#define MODE "Serial2 HS MODE"
	#elif COMM_DEBUG == 3
		#define SERIALDEBUG Serial3
		#define MODE "Serial3 HS MODE"
	#elif COMM_DEBUG == 4
		#define SERIALDEBUG BT
		#define MODE "BT Mode SoftwareSerial"
	#endif
	

	#ifdef DEBUG
			#define  DUMP(s, v)  { SERIALDEBUG.print(DEBUG_STARTCMD); SERIALDEBUG.print(F(s)); SERIALDEBUG.print(v); SERIALDEBUG.print(DEBUG_ENDCMD); }
			#define  DUMPS(s)    { SERIALDEBUG.print(DEBUG_STARTCMD); SERIALDEBUG.print(F(s)); SERIALDEBUG.print(DEBUG_ENDCMD);}
			#define  DUMPPRINTLN() { SERIALDEBUG.println();}
	#else
		#define  DUMP(s, v)
		#define  DUMPS(s)
		#define  DUMPPRINTLN() 
	#endif

	//Calibration functions in DEBUG
	#ifdef DEBUG_CAL
			#define  DUMPCAL(s, v)  { SERIALDEBUG.print(DEBUGCAL_STARTCMD); SERIALDEBUG.print(F(s)); SERIALDEBUG.print(v);SERIALDEBUG.print(DEBUGCAL_ENDCMD); }
			#define  DUMPSCAL(s)    { SERIALDEBUG.print(DEBUGCAL_STARTCMD); SERIALDEBUG.print(F(s)); SERIALDEBUG.print(DEBUGCAL_ENDCMD);}
			#define  DUMPDEBUGCAL(s, v)  { SERIALDEBUG.print(DEBUG_STARTCMD);SERIALDEBUG.print(F(s)); SERIALDEBUG.print(v);SERIALDEBUG.print(DEBUG_ENDCMD); }
			#define  DUMPSDEBUGCAL(s)    { SERIALDEBUG.print(DEBUG_STARTCMD);SERIALDEBUG.print(F(s)); SERIALDEBUG.print(DEBUG_ENDCMD);}
			#define  DUMPSAVAILABLE()    SERIALDEBUG.available()
			#define  DUMPREADSTRING() {SERIALDEBUG.readString();}
			#define  DUMPREAD(str) { str =SERIALDEBUG.read();}
	#else
		#define  DUMPCAL(s, v)
		#define  DUMPSCAL(s)
		#define  DUMPDEBUGCAL(s, v)
		#define  DUMPSDEBUGCAL(s)    
		#define  DUMPSAVAILABLE() false
		#define  DUMPREADSTRING()   
		#define  DUMPREAD(str) 
	#endif

	#ifdef SENDCOLOR
	  #define  DUMPCOLOR(code) { SERIALDEBUG.print(COLOR_STARTCMD); SERIALDEBUG.print(code);SERIALDEBUG.print(COLOR_ENDCMD);}
	#else
	    #define  DUMPCOLOR(code) { }
	#endif

    // Frequency setting defines
	#define TCS3200_FREQ_HI  0 // 100% prescaler
	#define TCS3200_FREQ_MID 1 // 20%
	#define TCS3200_FREQ_LO  2 // 2%
	#define TCS3200_FREQ_OFF 3 // off

	// Indices for any RGB data/ filter selection
	#define TCS3200_RGB_R  0
	#define TCS3200_RGB_G  1
	#define TCS3200_RGB_B  2
	#define TCS3200_RGB_X  3 // 'Clear' filter selection

	#define RGB_SIZE  4 // array index counter limit
    #define SIZENAME 10
    #define SIZECOLORS 8
    typedef struct{
		float value[RGB_SIZE];  // Raw data from the sensor
	} sensorData;

	typedef struct{
		uint8_t value[RGB_SIZE]; // the evaluated colour data (RGB value 0-255 or other)
	} colorData;
	typedef struct	{
		char    name[SIZENAME];  // color name 
		colorData rgb;    // RGB value
	} colorTable;

	class TCS3200 {
		public:

			uint8_t   _OUT;    // output enable pin
			uint8_t   _S0, _S1; // frequency scaler
			uint8_t   _S2, _S3; // photodiode filter selection
			uint8_t   _LED;
			uint8_t   _freqSet; 
			uint8_t   _nEEPROM=0;
			int 	  _lastColor = 0;
			int	      _nSamples = 10;
			char	  _ID[SIZENAME];
			
			colorTable _ct[SIZECOLORS]={
				colorTable {"WHITE", {255, 255, 255} },
				colorTable {"BLACK", {0, 0, 0} },
				colorTable {"YELLOW", {250, 250, 225} },
				colorTable {"ORANGE", {250, 220, 200} },
				colorTable {"RED", {240, 175, 190} },
				colorTable {"GREEN", {175, 200, 195} },
				colorTable {"BLUE", {150, 190, 220} },
				colorTable {"BROWN", {200, 180, 120} }
			};
  
			bool _interruption = false;

			unsigned long currentMillis,oldMillis;
			unsigned long refreshTime= 50; 

			sensorData  _raw;    // current raw sensor reading
			
			//Calibration white and Black
			sensorData  _relraw;    // current relative raw sensor reading
			sensorData  _darkraw;    // Dark Calibration values
			sensorData  _whiteraw;    // White Calibration values

			colorData _rgb;   // colour based data for current reading
			sensorData  _relrgb;    // current relative raw sensor reading
				
			TCS3200(){
  
			};
			TCS3200(uint8_t S2, uint8_t S3, uint8_t OUT, uint8_t nEEPROM = 0 ){
				_S2 = S2;
				_S3 = S3;
				_OUT = OUT;
				_freqSet = TCS3200_FREQ_HI;
				_nEEPROM = nEEPROM;
			};

			TCS3200(uint8_t S2, uint8_t S3, uint8_t OUT,uint8_t LED, uint8_t nEEPROM = 0 ){
				_S2 = S2;
				_S3 = S3;
				_OUT = OUT;
				_LED = LED;
				_freqSet = TCS3200_FREQ_HI;
				_nEEPROM = nEEPROM;
			};
			TCS3200(uint8_t S2, uint8_t S3, uint8_t OUT , uint8_t S0, uint8_t S1,uint8_t LED, uint8_t nEEPROM = 0 ){
				_S0 = S0;
				_S1 = S1;
				_S2 = S2;
				_S3 = S3;
				_OUT = OUT;
				_LED = LED;
				_freqSet = TCS3200_FREQ_HI;
				_nEEPROM = nEEPROM;
			};

			//  Data handling
			void  setPins(uint8_t S2, uint8_t S3, uint8_t OUT,uint8_t LED, uint8_t nEEPROM = 0){
				_S2 = S2;
				_S3 = S3;
				_OUT = OUT;
				_LED = LED;
				_freqSet = TCS3200_FREQ_HI;
				_nEEPROM = nEEPROM;
			};

			void  setPins(uint8_t S2, uint8_t S3, uint8_t OUT,  uint8_t S0, uint8_t S1,uint8_t LED, uint8_t nEEPROM = 0 ){
				_S0 = S0;
				_S1 = S1;
				TCS3200::setPins( S2,  S3,  OUT, LED,  nEEPROM );
				/*_S2 = S2;
				_S3 = S3;
				_OUT = OUT;
				_LED = LED;
				_freqSet = TCS3200_FREQ_HI;
				_nEEPROM = nEEPROM;*/
			};

			void  setID(String ID){
				ID.toCharArray(TCS3200::_ID, SIZENAME);
			};

			void  begin(){
				SERIALDEBUG.begin(BPS);
				SERIALDEBUG.println("Serial DEBUG is working");
				DUMPS(MODE);
				#ifdef DEBUG_CAL
					DUMPS(" CALIBRATION DEBUG MODE ");
				#endif
				
				//if (_S0 != NULL )pinMode(_S0,OUTPUT);
				//if (_S1 != NULL )pinMode(_S1,OUTPUT);
				if (_S0 != 0 )pinMode(_S0,OUTPUT);
				if (_S1 != 0 )pinMode(_S1,OUTPUT);
				pinMode(_S2,OUTPUT);
				pinMode(_S3,OUTPUT);
				pinMode(_LED,OUTPUT);
				pinMode(_OUT,INPUT);

				LEDON(true);
				setFrequency(_freqSet);
			};        // used to initialise hardware

			void  LEDON(bool ledON){
				digitalWrite(_LED, (ledON) ? HIGH : LOW );
			};
			void  nSamples(int nSamples){_nSamples = nSamples;}
			void  setEEPROMaddress( uint8_t nEEPROM ){
				TCS3200::_nEEPROM = nEEPROM;
			};
			
			void  voidRAW(sensorData *d) {
  
				for (int i = 0; i < RGB_SIZE; ++i)
				{
				d->value[i] = 0;
				}
			};

			void  setRefreshTime(unsigned long refreshTime){
				TCS3200::refreshTime = refreshTime;
			};

			void  setOutInterrupt(bool ON){
				if (ON){

				}else{

				}
			};
			// set the photodiode filter
			void  setFilter(uint8_t f){
				// set the sensor color filter
				switch (f){
					case TCS3200_RGB_R:   digitalWrite(_S2, LOW);   digitalWrite(_S3, LOW);   break;
					case TCS3200_RGB_G:   digitalWrite(_S2, HIGH);  digitalWrite(_S3, HIGH);  break;
					case TCS3200_RGB_B:   digitalWrite(_S2, LOW);   digitalWrite(_S3, HIGH);  break;
					case TCS3200_RGB_X:   digitalWrite(_S2, HIGH);  digitalWrite(_S3, LOW);   break;
					default:  DUMP("Unknown filter option", f); break;
				}
			}; 
			// set frequency prescaler - default 100%	
			void  setFrequency(uint8_t f){
				_freqSet = f;
				switch (f){
					case TCS3200_FREQ_HI:  DUMPS("HI");  digitalWrite(_S0, HIGH);  digitalWrite(_S1, HIGH);  break;
					case TCS3200_FREQ_MID: DUMPS("MID"); digitalWrite(_S0, HIGH);  digitalWrite(_S1, LOW);   break;
					case TCS3200_FREQ_LO:  DUMPS("LO");  digitalWrite(_S0, LOW);   digitalWrite(_S1, HIGH);  break;
					case TCS3200_FREQ_OFF: DUMPS("OFF"); digitalWrite(_S0, LOW);   digitalWrite(_S1, LOW);   break;
					default:  DUMP("Unknown freq option", f); break;
				}
			};

			// synchronously non-blocking reading value
			void read(bool RGB = true){
				TCS3200::currentMillis = millis();
				if(TCS3200::currentMillis-TCS3200::oldMillis >= TCS3200::refreshTime){

				if(RGB){
					TCS3200::readRGB();
				}else{
					TCS3200::readRAW();
				}

				//DUMP("\nNon Blocking system Time: ",TCS3200::currentMillis-TCS3200::oldMillis);
				//DUMPS("\n");

				TCS3200::oldMillis  = TCS3200::currentMillis;

				}
			};

			bool onChangeColor(){
				TCS3200::read();
				int cli= TCS3200::checkColor( &_rgb );

				if(cli != TCS3200::_lastColor){

					TCS3200::_lastColor = cli;
					#if SENDCOLOR
						TCS3200::sendColor();
					#endif
					return true;
				}else{
					return false;
				}
			};
			//Single Reading
			sensorData  color(){
				sensorData sensorcolor;   

				for (int i = 0; i < RGB_SIZE; ++i){

					float value = 0;
					float fvalue = 0;
					int n = 0;

					for (int j = 0; j < _nSamples; ++j){
						setFilter(i); 
						float color = pulseIn(_OUT, digitalRead(_OUT) == HIGH ? LOW : HIGH) ;

						// 1.5 value of threshold
						if (color/_nSamples > fvalue / 1.5) {

							fvalue = (fvalue + color)/_nSamples;
							value = value + color;
							n++;
						}

						sensorcolor.value[i] = value/n;  

					}

				}

				/*DUMP(" Red: ", color.value[0]);
				DUMP(" Green: ", color.value[1]);
				DUMP(" Blue: ", color.value[2]);  */
				return sensorcolor;
			};
			sensorData  relativeColor(bool RGB = true){

				if (RGB){
					uint32_t sumcolor = _rgb.value[0]+_rgb.value[1]+_rgb.value[2];
					_relrgb.value[TCS3200_RGB_R] = _rgb.value[TCS3200_RGB_R]/sumcolor;
					_relrgb.value[TCS3200_RGB_G] = _rgb.value[TCS3200_RGB_G]/sumcolor;
					_relrgb.value[TCS3200_RGB_B] = _rgb.value[TCS3200_RGB_B]/sumcolor;
					//_relraw.value[TCS3200_RGB_X] = _raw.value[TCS3200_RGB_X]/sumcolor;

					DUMP(" RelRed : ",_relrgb.value[TCS3200_RGB_R]);
					DUMP(" RelGreen : ",_relrgb.value[TCS3200_RGB_G]);
					DUMP(" RelBlue : ",_relrgb.value[TCS3200_RGB_B]);
					//DUMP(" RelClear : ",_relraw.value[TCS3200_RGB_X]);
					return _relrgb;
				}else{
					uint32_t sumcolor = _raw.value[0]+_raw.value[1]+_raw.value[2];
					_relraw.value[TCS3200_RGB_R] = _raw.value[TCS3200_RGB_R]/sumcolor;
					_relraw.value[TCS3200_RGB_G] = _raw.value[TCS3200_RGB_G]/sumcolor;
					_relraw.value[TCS3200_RGB_B] = _raw.value[TCS3200_RGB_B]/sumcolor;
					//_relraw.value[TCS3200_RGB_X] = _raw.value[TCS3200_RGB_X]/sumcolor;

					DUMP(" RelRed : ",_relraw.value[TCS3200_RGB_R]);
					DUMP(" RelGreen : ",_relraw.value[TCS3200_RGB_G]);
					DUMP(" RelBlue : ",_relraw.value[TCS3200_RGB_B]);
					//DUMP(" RelClear : ",_relraw.value[TCS3200_RGB_X]);
					return _relraw;
				}
			};
			// return RGB color data for the last reading
			void  getRGB(colorData *rgb){
				if (rgb == NULL)
				return;

				DUMPS("\ngetRGB");
				for (uint8_t i=0; i<RGB_SIZE; i++){
					rgb->value[i] = _rgb.value[i];
					DUMP(" ", rgb->value[i]);
				}
			}; 
			// return the raw data from the last reading
			void  getRaw(sensorData *d){
				// get the raw data of the current reading
				// useful to set dark and white calibration data

				if (d == NULL)
				return;

				DUMPS("\ngetRAW");
				for (uint8_t i=0; i<RGB_SIZE; i++){
					d->value[i] = _raw.value[i];
					DUMP(" ", d->value[i]);
				}
			};  
			// Read RAW Values
			sensorData readRAW(){
				//sensorData cl;
				sensorData rawcl;
				TCS3200::voidRAW(&rawcl);

				rawcl = TCS3200::color();
				_raw.value[TCS3200_RGB_R] = rawcl.value[0];
				_raw.value[TCS3200_RGB_G] = rawcl.value[1];
				_raw.value[TCS3200_RGB_B] = rawcl.value[2];
				_raw.value[TCS3200_RGB_X] = rawcl.value[3];

				DUMP(" Red : ",_raw.value[TCS3200_RGB_R]);
				DUMP(" Green : ",_raw.value[TCS3200_RGB_G]);
				DUMP(" Blue : ",_raw.value[TCS3200_RGB_B]);
				DUMP(" Clear : ",_raw.value[TCS3200_RGB_X]);
				DUMPPRINTLN();

				return rawcl;
			};
			// Read RGB Values
			colorData readRGB(){
				TCS3200::readRAW();
				colorData color = TCS3200::raw2RGB();

				DUMP(" Red : ",_rgb.value[TCS3200_RGB_R]);
				DUMP(" Green : ",_rgb.value[TCS3200_RGB_G]);
				DUMP(" Blue : ",_rgb.value[TCS3200_RGB_B]);
				return color;
			};
			
			void sendColor(){
				String code ;
				String str (TCS3200::_ID);
				if(str != ""){
					code += TCS3200::_ID;
					code += COLOR_SEPCMD;
				}
				code += TCS3200::_ct[TCS3200::_lastColor].name;
				DUMPCOLOR(code); 
			};      

			//Events for Calibration
			void  setDarkCal(){
				sensorData darkcl;
				DUMPSDEBUGCAL(" BLACK Calibration ");
				DUMPPRINTLN();
				TCS3200::voidRAW(&darkcl);
				bool sure= false;
				while (sure == false){

					while(!DUMPSAVAILABLE()){

					}
					DUMPREADSTRING();

					darkcl = TCS3200::readRAW();
					DUMPSDEBUGCAL("RGB BLACK Values"); 

					String dataRGB = "";
					for (int i = 0; i < RGB_SIZE; ++i){
						dataRGB += darkcl.value[i];
						dataRGB += DEBUGCAL_SEPCMD;
					}
					DUMPCAL("",dataRGB ); 
					DUMPPRINTLN();

					DUMPDEBUGCAL("",dataRGB);
					DUMPPRINTLN();
					DUMPSDEBUGCAL(" Are you sure this values are correct for BLACK Calibration? (Y/N)");
					while(!DUMPSAVAILABLE()){

					}
					DUMPPRINTLN();
					char chr;
					DUMPREAD(chr);
					DUMPDEBUGCAL("Char Read : ",chr);
					if (chr == 'Y'){
						_darkraw = darkcl;
						sure = true;
					}
				}
				DUMPPRINTLN();
				DUMPSDEBUGCAL(" End BLACK Calibration");
			};

			void  setWhiteCal(){
				sensorData whitecl;
				DUMPPRINTLN();
				DUMPSDEBUGCAL(" WHITE Calibration ");
				TCS3200::voidRAW(&whitecl);
				bool sure= false;
				while (sure == false){

					while(!DUMPSAVAILABLE()){

					}
					DUMPREADSTRING();

					whitecl = TCS3200::readRAW();
					DUMPSDEBUGCAL("RGB WHITE Values"); 

					String dataRGB = "";
					for (int i = 0; i < RGB_SIZE; ++i){
						dataRGB += whitecl.value[i];
						dataRGB += DEBUGCAL_SEPCMD;
					}
					DUMPCAL("",dataRGB ); 
					DUMPDEBUGCAL("",dataRGB);
					DUMPPRINTLN();
					DUMPSDEBUGCAL(" Are you sure this values are correct for WHITE Calibration? (Y/N)");
					while(!DUMPSAVAILABLE()){

					}
					DUMPPRINTLN();
					char chr;
					DUMPREAD(chr);
					DUMPDEBUGCAL("Char Read : ",chr);
					if (chr == 'Y'){
						_whiteraw = whitecl;
						sure = true;
					}
				}

				DUMPSDEBUGCAL(" End WHITE Calibration");
			};

			void  calibration(uint8_t nEEPROM = 0){
				TCS3200::setDarkCal();
				TCS3200::setWhiteCal();
				DUMPREADSTRING();

				DUMPSDEBUGCAL("\nDo you want to save Calibration in EEPROM?");
				while(!DUMPSAVAILABLE()){
				}

				char readY;
				DUMPREAD(readY);
				DUMPDEBUGCAL("Char Read : ",readY);
				if (readY == 'Y'){
				DUMPDEBUGCAL("\nBlack and White Calibration saved in EEPROM Address:  ",TCS3200::_nEEPROM);
				DUMPPRINTLN();
				TCS3200::saveCal(nEEPROM);
				}

				DUMPREADSTRING();DUMPSDEBUGCAL("\nDo you want to set Color values (Y) or Load EEPROM Values (N)?");
				while(!DUMPSAVAILABLE()){
				}

				DUMPREAD(readY);
				DUMPDEBUGCAL("Char Read : ",readY);
				if (readY == 'Y'){
				DUMPSDEBUGCAL("\nSetting RGB Values");
				TCS3200::setColorCal();
				DUMPREADSTRING();DUMPSDEBUGCAL("\nDo you want to save Calibration Colors in EEPROM?\n");
				while(!DUMPSAVAILABLE()){
				}

				DUMPREAD(readY);
				DUMPDEBUGCAL("Char Read : ",readY);
				if (readY == 'Y'){
				DUMPDEBUGCAL("\nColour table saved in EEPROM in EEPROM Address:  ", TCS3200::_nEEPROM);
				TCS3200::saveCT(TCS3200::_nEEPROM);
				}else{
				DUMPSDEBUGCAL("\nNot saved in EEPROM");
				}
				}else if (readY == 'N'){
				DUMPSDEBUGCAL("\nEEPROM RGB Values");
				TCS3200::loadCT( nEEPROM );
				}
			};

			void  setColorCal(){
				DUMPSDEBUGCAL(" Color Calibration ");

				while(!DUMPSAVAILABLE()){

				}
				DUMPREADSTRING();
				for (int i = 0; i < SIZECOLORS; ++i){
					bool sure= false;
					while (sure == false){
						DUMPPRINTLN();
						DUMPDEBUGCAL("Calibration for color - ", _ct[i].name);
						DUMPPRINTLN();
						colorData colorcl = TCS3200::readRGB();

						String dataRGB = "";
						for (int i = 0; i < RGB_SIZE; ++i){
							dataRGB += colorcl.value[i];
							dataRGB += DEBUGCAL_SEPCMD;
						}
						DUMPCAL("",dataRGB );
						DUMPPRINTLN();
						DUMPSDEBUGCAL("Is Correct? ");
						while(!DUMPSAVAILABLE()){
						}

						char readY;
						DUMPREAD(readY);
						DUMPDEBUGCAL("Char Read : ",readY);
						DUMPPRINTLN();

						if (readY == 'Y'){
							_ct[i].rgb = colorcl;
							sure = true;
						}else if(readY == 'N'){
						//sure = true;
						}
						DUMPREADSTRING();
					}
				}
			};

			// convert raw data to RGB
			colorData  raw2RGB(void)  {
				// Exploiting linear relationship to remap the range 

				int32_t x;
				colorData color;
				for (uint8_t i=0; i<RGB_SIZE; i++){
				x = (_raw.value[i] - _darkraw.value[i]) * 255;
				x /= (_whiteraw.value[i] - _darkraw.value[i]);

				// copy results back into the global structures
				if (x < 0) color.value[i] = 0; 
				else if (x > 255) color.value[i] = 255;
				else color.value[i] = x;
				}
				_rgb = color;
				return color;
			};

			uint8_t checkColor(colorData *rgb){
				int32_t   d;
				uint32_t  v, minV = 999999L;
				uint8_t   minI=0;

				for (uint8_t i=0; i< SIZECOLORS; i++){
					v = 0;
					for (uint8_t j=0; j<RGB_SIZE; j++){
						d = _ct[i].rgb.value[j] - rgb->value[j];
						v += (d * d);
					}
					if (v < minV) {
						minV = v;
						minI = i;
				}
				if (v == 0)   // perfect match, no need to search more
					break;
				}
				//DUMP("Color Checked : ", minI);
				return(minI);
			};

			//EEPROM Saving Values
			void  saveCal(uint8_t nEEPROM = 0){
				_nEEPROM = nEEPROM;
				int calWhiteAddress = _nEEPROM;
				int calDarkAddress = calWhiteAddress + sizeof(sensorData);
				EEPROM.put(calWhiteAddress, _whiteraw);
				EEPROM.put(calDarkAddress, _darkraw);

				DUMPS("\t White Calibration: ");
				for (uint8_t i=0; i<4; i++){
					DUMP(" Color: ", _whiteraw.value[i]);
				}
				DUMPS("\n \t Black Calibration: ");
				for (uint8_t i=0; i<4; i++){
					DUMP(" Color: ", _darkraw.value[i]);
				}

				DUMPS("\nSaved Calibration");
			};
			void  loadCal(uint8_t nEEPROM = 0){
				TCS3200::loadBW(nEEPROM);
				TCS3200::loadCT(nEEPROM);
			};

			void  loadBW(uint8_t nEEPROM = 0){
				_nEEPROM = nEEPROM;
				int calWhiteAddress = _nEEPROM;
				int calDarkAddress = calWhiteAddress + sizeof(sensorData);

				EEPROM.get(calWhiteAddress, _whiteraw);
				EEPROM.get(calDarkAddress, _darkraw);

				DUMPS("\n \t White Calibration: ");
				for (uint8_t i=0; i<4; i++){
					DUMP("Color: ", _whiteraw.value[i]);
				}
				DUMPS("\n \tBlack Calibration: ");
				for (uint8_t i=0; i<4; i++){
					DUMP("Color: ", _darkraw.value[i]);
				}
				DUMPS("\n Calibration Loaded ");
			};
			void  saveCT(uint8_t nEEPROM = 0){
				int address = nEEPROM + 2*sizeof(sensorData);

				for (int i = 0; i < SIZECOLORS; ++i){
					EEPROM.put(address, _ct[i]);
					address += sizeof(colorTable);
				}
				TCS3200::readCT();

				DUMPS("\n Color Table Calibration Saved ");
				DUMPPRINTLN();
			};
			void  loadCT(uint8_t nEEPROM = 0){
				int address = nEEPROM + 2*sizeof(sensorData);

				for (int i = 0; i < SIZECOLORS; ++i){
					EEPROM.get(address, _ct[i]);
					address += sizeof(colorTable);
				}
				TCS3200::readCT();

				DUMPS("\n Color Table Calibration Loaded ");
			};
			void  readCT(){
				for (int i = 0; i < SIZECOLORS; ++i){
					DUMP("\nColor: ",_ct[i].name);
					for (uint8_t j=0; j<3; j++){
						DUMP("  -  ", _ct[i].rgb.value[j]);
					}
					DUMPS("\n");
				}
			};
	};


#endif