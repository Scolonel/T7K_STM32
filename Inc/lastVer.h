54
// ������ - 3 05.04.2022 - ������ �������� , ���������� ������� � 1210 � 850 ��� ���� ���� ����
// ������ - 6 22.07.2022 - ��������� ����������, ��������� ��������� �� ���������, ��������� ��������� ��� ����� ����������� 
// �����. ������ ������
// ������ - 8 02.08.2022 - ����������� ��������� ����� � ������ ������_������� 
// ������ - 9 10.08.2022 - ������������� � ������ ������_������� �� ����� ������
// � ���� ��������� ���� ����������� ���������� �������������...
// ������ - 10 12.08.2022 - ������������� � ������ ������_������� �� ����� ������ 
// ����������� ������, ���������� ��������� ���������� �����,
// �������� �� ���� ����������� - �����, ��� ���������� �������� (2222)
// �������� ���������������� �� ������� DO � 10��� �� 120��
// �������� � ������ 2 ����� � 10�� ������� �� 6.2�� �.�. ������� ����� 3 ������...
// ������ ���� ����������� ��������� - �������� ���������������� �������� 2���
// ��������� ������� 5� ������ ����� ��������� �����... 
// ������ 4 ���������, �� ��������� ����, ������� ��������� ���! ����!
// ������ - 11 16.08.2022 - �� ��������� ���������� ���������, ��������
// ���������� ��������� ��������� � 3.2 �� 4.2 ���������� �����-���������
// ����� 4.2 ������ "�������"
// ������ 3.2 ����� � ��������� OFF -  ����� ����������� ��� �������-��
// ������� WD!
// ���������� ��������� �.�.10-11
// ������ - 12 18.08.2022 - 
// �� ���� ���������� ��������� ���� ������������, ���� �� ������� ��� �� ��������
// ��� ���� ������������ ������� ��� ���������� ����� ���� �� ������ ����� 
// �������� 850, � ������� CW* ORL �� ��������� ������������ ��������� - ������ ������ ����������
// ��� ������������, ��������� ���������� 
// ������ ;SET:YEAR?<CR>
// ��������� ;SET:YEAR 22<CR>
// ������ - 13 19.08.2022 - 
// "��������" ��� ������������ � ��������, � ������� �������, ��� ���� ����� ���� ��������.
// � ���������� UART2 (����������) ���������� ���������� ������� �� ��������������- ���������, 
// � ����������� �������� �������������� ������ ������.
// ����� ������� ������������ I2C ��� STM32F103 ��� ��� ��������� "���������"
// ������ - 14 22.08.2022 - 
// ���������� ���������� ����� � ��������, 
// ����� �� ������� ����� � ��������, ������ ���������, ��� ������ ������� "�������"
// �������� ��-�� �� ������������ �������...
// ������ - 15 22.08.2022 - 
// ������������� ������ ������ ��� �������� ��������� ������ ���� ����, 
// ��� ����������� (���)
// ������ - 16 22.08.2022 - 
// ������ ������� ���������� ���������� ����� �� ���� ������ P1a, 
// �������������� �������� ������ "�����" � "W,dBm,dB" 
// ��� ���� �������������� �������� ������� � ��������� -3.0 ... +3.0 dBm
// ������ - 17 22.08.2022 - 
// ��������� ������ ������������ (��� ����������) ������ � ������ P1a, ��� � ������� �������
// ��������� ����������� �� 3 ������, ��� ��� ����� ����� 512 �������
// ������ - 18 23.08.2022 - ���������� �������������� ��������� ������ ������ ������ � ������ �������
// ������ - 19 03.10.2022 - ��������� ��������� ���������� ���������� ������ � �������� ������ �����, ��� ��� ��� �� ���������
// ��������� � ������������� �� ��������� ��������� ����������� ������� ����� ����������
// ������������� ������ ��� ����������
// ��������� ������ ������� � ���������� ��������� ����� � ����� �������� �� 0.01, 
// ��������� ����� � ���� �� 0.1 
// ����������� ��� ������ ����������������! ����� ������� �������� ������������� ORL
// !!!!...�������� ����������...!!!!
// � ������� ��������� � �������� ����� ��� ���������!
// ������ - 20 04.10.2022 - ������� ���������� ��������,
// ������ �������� ������ ;RMT:MOD?<CR>
// ����� �������� �����! <CR> - ���� �������, 
// ERR<CR> - ���� ������
// �������� ������ ������
// N- ������� ����� ������ �������
// 1� � P1, 2� =P1a, 3� = Sc, 4� - P2, 5�-ORL, 6� -Ln, 8� - TESTERAUTO, 9�-RED LS, 11�- Setup, 0-����� ������ 
// BatLevel -  ������� ������� � ��������� ������ �������
// � ��������� ������ ����������� ���� ��� �� ������ � ������ ������
// ��������� ���������� ������ ;RMT:MOD N<CR>
// N<CR> - ���� �������,
//  10: // P1 dBm
//  11: // P1 dB
//  12: // P1 W
//  20: // P1a dBm
//  21: // P1a dB
//  22: // P1a dBe
//  30: // Sc OFF
//  31: // Sc CW
//  32: // Sc 270 Hz
//  33: // Sc 2 kHz
//  34: // Sc CW*
//  40: // P2 dBm
//  41: // P2 dB
//  50: // ORL
//  60: // Ln
//  61: // Ln start
//  90: // RE OFF
//  91: // RE CW
//  92: // RE 1Hz
//
// ERR<CR> - ���� ������ ��� ����� �� ����������,
// ����������� ������, 1x-6x � 9x.
// ������� ��������� ����� ����� � P1, Sc, P2, ORL, 
// ;RMT:LW N<CR>, N - ����� ����� � ��
// ����� // N<CR> - ���� �������, � ����������� �� ������
// ERR<CR> - ���� ������
// ��� P1 ����� ����� ��������������� ������ 5 � ��������� 800-900 1210-1650 
// ���� �� ������ 5 �� ����������� �� 5 ������ ������������� ����� �����
// ��� P2 - �������������� ������ ����� ���� ���������� 
// ��� Sc � ORL ������ ����� ���� ������������� ����������
// ������ - 21 05.10.2022 - ������� ���������� ��������,
// ���� ������� � ������ ������������� � ������������ � ������� � ������� 
// ��� ������� ���� 
// ������ - 22 25.10.2022 - ������� ���������� ���������� � ��� ���������� ������� � ��������� �������
// � DrawFunctions.c       // ���������� ������� ���� ����������  void DrawMainModes (void)
// ��� ���������� ������ � ���������� �����������, ������� �� main 
// ������ - 23 01.11.2022 - ��� �������� � ���������� F303E, ������� VDDA - �� ��������������� �������� ����� ����
// ����� � ���������� ������ ��� 3.0 ������, � ���� ����� ����� ifdefine ��� ������ ��������� ��� 4�� �� ��������!
// ���������� ���������� �����!!! ������ R67 DA7 ������ L5 ���������� ���� STPS1L30U ������ � VCC+3V
// 1667296800
// ������ - 24 01.11.2022 - �������� ������� �������� ��������� CheckUbat(void) � DrawFunctions.c �� main
// ������ - 25 02.11.2022 - ����� �������� �.�. �� ������ ����� � ����� ������...
// ������ - 26 02.11.2022 - ��� ORL �������� ��������� ������ 60 �� , ������ ������� �� 60
// ������ - 27 09.11.2022 - ��������� ��������� ���������� ���������� ����� � ������ 303
// ������ - 28 28.11.2022 - ��������� ������ 20.5 � ������� ������������ L � R ���� ������� ����
// ������ - 29 29.11.2022 - ������������� ����� ����� ����� � ���������� P1  
// ����� ��������� �� ���������� ����� ����������
// ������� ������� ����������� P2 � dBm �� ������ ����� ����� ��� ������������
// ;PM:CNTRL:DP2?<CR>
// ����� P2dBmValue, OrlCalkFree,PowP2_mW,PowClbr_mW
// �2 � ���, ���������� ORL ��� �����������, �������� �������� �2 � ���, �������� � ���
// ������� ��������� ����� *SETUPEN
// ������ - 30 07.12.2022 - ��� �������� �� UART ����������� ����������� ���������� 
// ��������������� ���������� ���������� ���������� � ���������� � ����������� �������  
// ������ - 31 09.12.2022 - ������� �������� ���������� ����� � ������ �������
// ���� ������ ������ � ������ ��������� �� ������...
// ������ - 32 09.12.2022 - ������� ������ ���� � ������������� �������, ���� �� ������������ � TabSG1
// ���������� ������ �� ��������
// ������ - 33 09.12.2022 - ��� ��������� ����� ������� ����������� ����������� ��� ���������� ���������
// ������� ������ ������ ����������� �� ��������� ��������� ���� ��� ����, � ���������� ������� ���������, 
//  ������ � ������ ������� ����������, 
// �� ��������� ���� ���� ��������� ��������� �� ���������� ���������������� ������ !>
// ������ - 34 12.12.2022 -  ���� ������ ���������� �� ��� ������ � �������� UART2 (����������) �� ��� ���������� 
// �� �������� 9600 �� 1200 �������� � � AL ����� ���� ���� �� �������� ��������!!!
// ������ - 35 09.01.2023 -1673258400 ����� ����� ����!
// ������ - 36 09.01.2023 -1673258400 ������ ����������������� ������������ �������,
// ����� ������ ���, ���� ��������� ���� �� 1 �������� � ��� ������� ��� CW*
// !!! �������� !!!
// ����� � ������������� F303  - ����� ����� 3.0V ( ������� �� ������������ ���)
// ����������� F103�  � GD23F303E(��������) , ��������� ��� �������� �� ����� 2.5�, ������� ��� 
// ��������� ����� LM4040-2.5 !!! ��� ����� ��� ��������� ������ ��� ������� ����� ������������ �� ������
// � �� ������������� ������ ������ ����� ��� ������� ���������!
// ������ - 37 13.02.2023 -1676282400 ������ �� �������������� ������� ��������� � ORL,
// �� ������ � ��������� ������ ������������� ������ �� �.�
// �������� �� ��������� 4 ���
// �� ������ dB/dBm/mW ��������������� �������������� �������� ��� ����������
// ������ - 38 17.02.2023 -1676628000 ������ �� �������������� ������� ��������� � ORL,
// ������������� �������� "�����" � "������" , "���" ��� ������ ������� � �2 ������ -47 ��� (!!! ��� �� 47 �� �� ORL!!!)
// �������� ������������� ����, ����� �������� ������ ���/��\���
// "����" ��� ���������� � ��������� 13-17 �� �� ORL �������� ������������ �� 14.7
// ��������� ��������������� ������� �� ������ ���/��\���, ��������� ��������������� ������������� ���� ������ � ����� ����
// ��� ������ ������������� ���������� ����� � ����� ����� � ����� ORL
// ��������� ������������� ������������ ����� ������ ������� � ������ ���� ������ ��������� ������
// "���" (<-47���) - ������ "R" �� ������ �������
// "����" (13-17 �� ORL) - ������ "C" , � ������� �������
// ������ - 39 01.03.2023 -1676628000 
// ��������� ������  - ������ ��������� �������� ������ ��� ���������� ����������� � �������
// P1, P1a, ORL
// ������� ������� ������� ����� *.bmp ����������
// ;RMT:SCR<CR>
// � ����� ���������� ���� �������� ������ ��������� �������, � ������� BMP 128*32
// ������ - 40 10.03.2023 -1676628000 
// ����� ���������� �����, �������� �������� ����������, �������� � ��������� ��������� "�������� �����"
// ������ ������, ���� ����� ����� �� ����� 2 ��� 3 ���� ���� ������� ����
// �.�. ������ ���������� ����������� ���������
//
// ������ ��������� ��� ��������� � ��������������
// ������ ��������� ��������� ��������� � ������ �������
// 0 - ������ ����� ������ �������� ������, �������� �������� �����
// 1 - ������ ����� ������ �������� ������, �������� �������� �����
// 2 - ������ ����� ������ �������� ������, �������� �������� �����
// 3 - ��������� �������� ��������, ������� �������� ���������� �������
// 4 - �������� ���������� ������, �� �������� ��������
// 5 - ����� ������� ��� ����� �������� ��������, ������ � ������ ��������� ��������
// 6 - ������ �������� �������� �� ������, � ��� ����� ������� �� �������� ������ ����, ������ ����� �����
// 7 - ������ ���� ����,
// 8 - ������ ���� ����, 
// 9 - ���� ����� �� ���������������� ���� ���������� ���������, ������� ��������� �������� ������ ���������
// 10 - ������ ����������� � ������, ���������� �������� ORL � ������� ���������
// 11 - ��������� �������� �������� ����������� ORL �������  ������� ������
// 12 - ������ �������� ORL , ���� ����
// 13 - ������ �������� ORL , ���� ����
// 14 - ����������� �������� ������ �������� ORL �� ������, ����������� ��������� �������� �����
// ������ - 41 15.03.2023 -1678874400 
// ���������� ��������� ��������� ��������� � ������ �������
// ������ - 42 15.03.2023 - ������� ������ ���� � ������������� �������, ���� ������� � ��������
// �� ������� ��� 7326 ��������, ������������� TabSG1
// ������ - 43 28.03.2023 - ������, ��� ��������� ORL ��� ����� ������ �������� ����������, ������������� ������ "�"
// ���������� ��� ���������, ��� ����� �� UART �������� ������� � �������� ��������������, ��������� �� ����������!
// ������ - 44 26.04.2023 -1682503200
// ����� �� ����� � ������������� ������ �������� ������� ��� ������� �.�
// ����������� �������!!!
// 0,1,2,3,4,5,6,7,8,9,�,�,�,�,�,-,+,����������� A,L
// ������ - 45 05.05.2023 -1683280800
// ���������� ��������� �� 21.02.2023 , ��� ������ ���������� �� �������� �������� ������ SLAVE
// ��������� ���� �� ���� ��������� ������� � ������������� � ������ �����. ����� ��������� ������ ������, ��� ��������
// ��������� ������... 
// ������ - 46 15.08.2023 -1692093600
// ������ ����� ���� ����������, ����� �������� 2/4/8/16 ����� ��������������, �� ��������� 30 �������� ����������� � ������� ����� ������
// ������� ����������� ���� ���������� ��������� ��������� ���� ������
// ������� ��������������� � ����������, ����� �����, ������ W/dB/dBm - ����������� ������� ��������� ���������� ���� ����������
// � �����. ���������� ���������
// ������ ������ ����������� ����� ����������...
// ����� � ����� ������ ������������� �������� ������ �����+���� ����� ����������� ����� ��������������...
// ����� ������� ����� DMA UART2 ��� ������ ������ ��������� ����, ��� ������ �� ���� ������������ �������.
// �������� ���� "���������" ��������� ��������, � ����� 103� ����������.
// ������ - 47 15.08.2023 -1692093600 -�� �� �����, ���� ���������������� main � ����� ������ ��� 103 � 303...
// ������ - 48 15.08.2023 -1692093600 �� �� �����, ��� �� ����� ���������� ��� ���������� � ������ P1a
// ������� ������ ���������  ���� ����  ���������� � ���� ������ �� ���������� ��� ������� P1
// ����� � ������� �� �������� ����� "4" ����� ���������, ��������
// ������ ��������� ������� "������� �� ����" ��� �������, ��� ������, ������ ����������� ����,
// ��� ��� ��� "����������" ����� �� �������� ������ ������� �� ����,
// ��������� ��������� ������� ������������ �������� ���������� �������� ���������� ������� �� ����� �������.
// ��������������� �������� ��������� ������ ������, �� ������� ���� ��� �������� � ��������� 103�
// ������ - 51 22.08.2023 -1692698400 - ������ � ������������ ������� ���������� ��� , ��� ������ ���������, ����������� ����������
// ���, ��� � ���� ������� ������� ������������ ��������� ��� �2, ������� ������������ �� ��������������� ������.
//�.�. - ��������� ��� ��������� �������� (�������� �����) � ������ �� �������������� ��� � ����� � ������ ����� ����������
// ���������� ������� ���������, ����� ������������ ��� ��������� ������ �2, � ������ � �����������, ����� ��� �������, 
// � ������������ ��� ���� ��������� � ��������� � �2, ��� ��� �� ����� �������� � ������ ��� ����������.
// ������� ������ �������� ��� ��������������� �������� ����� ������� ����� �������������� ��� �� � ���������������� �������...
// ����� ������ ��������� � ����� MAIN � ��� 303 ���������...
// ������ - 52 12.02.2024 -1707732000 - ������ � ������������ ������� � ���������� ����� � ���������� ���� � �������
// ������ - 53 01.03.2024 -1709287200 - �����! ����� ���� ��� ���������� �������, ���������� ������, � ������ ��������� 
// ���� �����������...
// ������ - 54 21.10.2024 -1709287200 - �����! ����� ���� ��� ���������� �������, ���������� ������, � ������ ��������� 
// ���� �����������...
