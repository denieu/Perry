//---------------------------------void onLeft---------------------------------

//Função que controla o motor esquerdo, recebe um valor entre -255 e 255
void onLeft(int controlAction) {
  controlAction = constrain(controlAction, -255, 255);

  if (controlAction == 0) {
    analogWrite(PINENA, 0);
  }
  else {
    if (controlAction > 0) {
      controlAction = controlAction + LEFTDEADBAND;
      digitalWrite(PININ1, HIGH);
      digitalWrite(PININ2, LOW);
      analogWrite(PINENA, controlAction);
    }
    else {
      controlAction = controlAction - LEFTDEADBAND;
      digitalWrite(PININ1, LOW);
      digitalWrite(PININ2, HIGH);
      analogWrite(PINENA, -controlAction);
    }
  }
}

//---------------------------------void onRight---------------------------------

//Função que controla o motor direito, recebe um valor entre -255 e 255
void onRight(int controlAction) {
  controlAction = constrain(controlAction, -255, 255);

  if (controlAction == 0) {
    analogWrite(PINENB, 0);
  }
  else {
    if (controlAction > 0) {
      controlAction = controlAction + LEFTDEADBAND;
      digitalWrite(PININ3, HIGH);
      digitalWrite(PININ4, LOW);
      analogWrite(PINENB, controlAction);
    }
    else {
      controlAction = controlAction - LEFTDEADBAND;
      digitalWrite(PININ3, LOW);
      digitalWrite(PININ4, HIGH);
      analogWrite(PINENB, -controlAction);
    }
  }
}

//---------------------------------void onMotors---------------------------------

//Função que controla os dois motores, recebe um valor entre -255 e 255 para cada motor, primeiro o motor esquerdo e depois o direito
void onMotors(int controlActionLeft, int controlActionRight) {
  onLeft(controlActionLeft);
  onRight(controlActionRight);
}
