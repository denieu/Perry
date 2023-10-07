//---------------------------------void tcrtRead---------------------------------

//Leitura analogica dos tcrts, valor de 0 a 1023, onde 0 é igual a 0V e 1023 igual a VCC
int readAnalogS[SENSORS] = {0, 0, 0};

//Função que realiza a leitura analogica dos sensores
void tcrtRead() {
  // for (int count = 0; count < SENSORS; count++) {
  //   readAnalogS[count] = analogRead(PINSENSORS[count]);
  // }
  readAnalogS[0] = analogRead(A3);
  readAnalogS[1] = analogRead(A2);
  readAnalogS[2] = analogRead(A1);

  Serial.print("Sensor 1: ");
  Serial.print(readAnalogS[0]);
  Serial.print(" | Sensor 2: ");
  Serial.print(readAnalogS[1]);
  Serial.print(" | Sensor 3: ");
  Serial.print(readAnalogS[2]);
}

//---------------------------------void tcrtMap---------------------------------

//Leitura com o map dos tcrts, valor normalizado de 0 a 100 baseado na calibração predefinida
float readMapS[SENSORS] = {0, 0, 0};

//Ajusta o valor da linha, conforme definido se está é branca ou preta
//Ajusta o valor caso a linha seja preta
#if defined LINECOLORBLACK
#define MAPVALOR1 100
#define MAPVALOR2 0
#endif
//Ajusta o valor caso a linha seja branca
#if defined LINECOLORWHITE
#define MAPVALOR1 0
#define MAPVALOR2 100
#endif

//Função que normaliza a leitura dos sensores, para um valor de 0 a 100, onde 100 é quando existe uma linha e 0 quando não existe
void tcrtMap() {
  tcrtRead();

  for (int count = 0; count < SENSORS; count++) {
    readMapS[count] = constrain(map(readAnalogS[count],  calibrationMinS[count],  calibrationMaxS[count], MAPVALOR1, MAPVALOR2), 0, 100);
  }

  Serial.print(" | MapSensor 1: ");
  Serial.print(readMapS[0]);
  Serial.print(" | MapSensor 2: ");
  Serial.print(readMapS[1]);
  Serial.print(" | MapSensor 3: ");
  Serial.print(readMapS[2]);
}

//---------------------------------void generateSensorOut---------------------------------

//Variavel que guarda a leitura anterior da posição da linha
float sensorOutAnt = 0;

//Função que gera o valor do sensorOut, que é a variavel controlada onde o objetivo é tentar sempre se manter no REF
float generateSensorOut() {
  //Faz e normaliza a leitura dos sensores
  tcrtMap();
  
  //Variavel que guarda quantos sensores estão na linha
  int sensorsInLine = 0;
  //Variavel que guarda o valor da posição da linha, entre 0 e 100
  float sensorOut = 0;

  //Varre todos sensores para ver quantos estão na linha, salva esse valor na variavel sensorsInLine
  for (int count = 0; count < SENSORS; count++) {
    if (readMapS[count] > MINLINEVALOR) {
      sensorsInLine++;
    }
  }
    Serial.print(" | sensorsInLine: ");
    Serial.print(sensorsInLine);
  //Se não existe nenhum robo na linha
  if (sensorsInLine == 0) {
    //Verifica se o valor do sensorOutAnt era maior que 90 ou menor que 10, se sim satura o sensorOut em 100 ou em 0, se não o mantem com valor anterior
    if (sensorOutAnt <= 10) {
      sensorOut = 0;
      sensorOutAnt = sensorOut;
      Serial.print(" | Sensor Out: ");
      Serial.println(sensorOut);
      return sensorOut;
    }
    else if (sensorOutAnt >= 90) {
      sensorOut = 100;
      sensorOutAnt = sensorOut;
      Serial.print(" | Sensor Out: ");
      Serial.println(sensorOut);
      return sensorOut;
    }
    else {
      sensorOut = sensorOutAnt;
      Serial.print(" | Sensor Out: ");
      Serial.println(sensorOut);
      return sensorOut;
    }
  }
  else {
    int sensorOutAux1 = 0;
    int sensorOutAux2 = 0;
     
    for (int count = 0; count < SENSORS; count++){
      sensorOutAux1 = sensorOutAux1 + (count * readMapS[count]);
      sensorOutAux2 = sensorOutAux2 + readMapS[count];
    }
    
    sensorOut = 100 / (SENSORS -1) * sensorOutAux1 / sensorOutAux2;
    sensorOutAnt = sensorOut;

    Serial.print(" | Sensor Out: ");
    Serial.println(sensorOut);
    return sensorOut;
  }
}
