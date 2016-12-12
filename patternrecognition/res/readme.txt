/**********************************************************************
 *  Mönsterigenkänning readme.txt
 **********************************************************************/

/**********************************************************************
 *  Empirisk    Fyll i tabellen nedan med riktiga körtider i sekunder
 *  analys      när det känns vettigt att vänta på hela beräkningen.
 *              Ge uppskattningar av körtiden i övriga fall.
 *
 **********************************************************************/
    
      N		brute       	sortering
 -----------------------------------------
    150		48 ms		34 ms
    200		120 ms		50 ms
    300		335 ms		87 ms
    400		786 ms		111 ms
    800		5889 ms		400 ms
   1600		47194 ms	1360 ms
   3200		405573 ms	6118 ms
   6400		3038168 ms	24820 ms
  12800		50000000 ms	107104 ms


/**********************************************************************
 *  Teoretisk   Ge ordo-uttryck för värstafallstiden för programmen som
 *  analys      en funktion av N. Ge en kort motivering.
 *
 **********************************************************************/

Brute:		O(n^4) Vi har 4 stycken nästlade for-loopar i värsta fall

Sortering:	O(n^2) För varje punkt i points anropas getlines() som också
		itererar över punkterna i points.
