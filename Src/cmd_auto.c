    //-------------------------------->>>>  SAWPW Калибровка (Master)
  case SavPw:
sprintf((char*)TX_Buf1, "####SMP%4u\r",L_W_S[Ind_LW]%3000);//

    //-------------------------------->>>>  ORL
  case Orl:                                   // (Master)
  case Sl_Orl:                                // (Slave)
sprintf((char*)TX_Buf1, "####SLA%4u %.2f\r",L_W_S[Ind_LW]%3000, PwS[Ind_LW]);//UUU

    //------------------------->>>> CALPW   ====     (Slave) (Калибровка)
  case CalPw: //  измерение мощности источника и ее передача
sprintf((char*)TX_Buf1, "####YP%.2f\r", Pow_SC_Tx);//UUU

    //--------------------------->>>> Px (Slave) Измерение мощности в автомате ь
  case Px:
sprintf((char*)TX_Buf1, "####YP%.2f\r", Pow_SC_Tx);//UUU

    //-------------------->>>> CmdSw
  case CmdSw:                                     // (Master)  
sprintf((char*)TX_Buf1, "####SWM\r");//UUU

    //-------------------->>>> AnsOK
  case AnsOK:                                     // (Slave)
sprintf((char*)TX_Buf1, "####SOK\r");//UUU

    //-------------------->>>> AskOrl
  case AskOrl:                                      // (Master)  
sprintf((char*)TX_Buf1, "####SRL\r");//UUU

    //-------------------->>>>  LnR // измерение длины
  case LnR:                                      // (Master)  
sprintf((char*)TX_Buf1, "####SWL\r");//UUU

    //-------------------->>>>  LnR_Clb // измерение длины
  case LnR_Clb:                                      // (Master)  
sprintf((char*)TX_Buf1, "####SWL\r");//UUU

    //--------------------->>>> AnsOrl
  case AnsOrl:                                      // (Slave)
sprintf((char*)TX_Buf1, "####SLO%4u %.2f\r",LW_BA[Cnt_SPw], ORL_BA[Cnt_SPw]);//UUU
