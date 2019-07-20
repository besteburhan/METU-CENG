#Don't judge, please :D
#İlk kodlama deneyimim : Ben de şaşkınım bu kod nasıl çalıştı, neden çalıştı .. :)

u=raw_input ()
t = u.find('?')
if len(u) != 8 or u[6] != "-":
	print "INVALID"
	quit()
if u.find('?') == -1:
	
    suma = int( u[0]) + int(u[2]) + int (u[4])+ int (u[7] )
    sumb = int ( u[1])*2
    sumbb = ((sumb%10) + sumb / 10)
    sumc = int (u[3])*2
    sumcc = ((sumc%10) + sumc / 10)
    sumd = int (u[5])*2
    sumdd = ((sumd%10) + sumd / 10)
    sumt = suma + sumbb + sumcc + sumdd
    
    if len(u) == 8 and u[6] == "-" and sumt%10 == 0 :
             print "VALID"
    else :
             print "INVALID"

else :
    if t==0 :
    
        sumx = int(u[2]) + int(u[4])+ int(u[7])
        y = int (u[1])*2
        sumyy = ((y%10) + y / 10)
        k = int (u[3])*2
        sumkk= ((k%10) + k / 10)
        AA = int (u[5])*2
        sumAAA= ((AA%10) + AA / 10)

        sumz = sumx + sumyy + sumkk + sumAAA
        x0= abs(100-sumz)%10
        pu = u[1:]
        print str(x0) + str(pu)
    elif t==1  :

        sumA = int (u[0]) + int(u[2]) + int (u[4])+ int (u[7] )
        B = int (u[3])*2
        sumB = ((B%10) + B / 10)
        C = int (u[5])*2
        sumC= ((C%10) + C / 10)

        sumzz = sumA + sumB + sumC
        if (100-sumzz)%10 == 0:
            x1=0
        elif  (100-sumzz)%10 == 2:
            x1=1
        elif  (100-sumzz)%10 == 4:
            x1=2
        elif  (100-sumzz)%10 == 6:
            x1=3
        elif (100-sumzz)%10 == 8:
            x1=4
        elif (100-sumzz)%10 == 1:
            x1=5
        elif (100-sumzz)%10 == 3:
            x1=6
        elif (100-sumzz)%10 == 5:
            x1=7
        elif (100-sumzz)%10 == 7:
            x1=8
        elif (100-sumzz)%10 == 9:
            x1=9
         

        ppu = u[0:1]
        Ppu = u[2:]
        print str(ppu) + str(x1) + str(Ppu)
    elif t==2 :
 
        sumD = int (u[0]) + int (u[4])+ int (u[7] )
        E = int (u[1])*2
        sumE = ((E%10) + E / 10)
        F = int (u[3])*2
        sumF= ((F%10) + F / 10)
        G = int (u[5])*2
        sumG = ((G%10) + G / 10)

        sum22 = sumD + sumE + sumF + sumG 
        x2= abs(100-sum22)%10
        ttu = u[0:2]
        Ttu = u[3:]
        print str(ttu) +  str(x2) + str(Ttu)
    elif t==3 :

        sumH = int (u[0]) + int(u[2]) + int (u[4])+ int (u[7] )
        I = int (u[1])*2
        sumI = ((I%10) + I / 10)
        K = int (u[5])*2
        sumK = ((K%10) + K / 10)

        sum33 = sumH + sumI + sumK
        if (100-sum33)%10 == 0:
            x3=0
        elif  (100-sum33)%10 == 2:
            x3=1
        elif  (100-sum33)%10 == 4:
            x3=2
        elif  (100-sum33)%10 == 6:
            x3=3
        elif (100-sum33)%10 == 8:
            x3=4
        elif (100-sum33)%10 == 1:
            x3=5
        elif (100-sum33)%10 == 3:
            x3=6
        elif (100-sum33)%10 == 5:
            x3=7
        elif (100-sum33)%10 == 7:
            x3=8
        elif (100-sum33)%10 == 9:
            x3=9
       
        rru = u[0:3]
        Rru = u[4:]
        print str(rru) +  str(x3) + str(Rru)

    elif t==4 :
               
        sumL = int (u[0]) + int(u[2]) + int (u[7] )
        M= int (u[1])*2
        sumM = ((M%10) + M / 10)
        N= int (u[3])*2
        sumN = ((N%10) + N / 10)
        O = int(u[5])*2
        sumO =((O%10) + O /10)

        sum44 = sumL + sumM + sumN + sumO
        x4= abs(100-sum44)%10
        ggu = u[0:4]
        Ggu = u[5:]
        print str(ggu) +  str(x4) + str(Ggu)

    elif t ==5 :

        sumP = int (u[0]) + int(u[2]) + int (u[4])+ int (u[7] )
        S = int (u[1])*2
        sumS = ((S%10) + S / 10)
        T = int (u[3])*2
        sumT = ((T%10) + T / 10)

        sum55 = sumP + sumS + sumT
        if (100-sum55)%10 == 0:
            x5=0
        elif  (100-sum55)%10 == 2:
            x5=1
        elif  (100-sum55)%10 == 4:
            x5=2
        elif  (100-sum55)%10 == 6:
            x5=3
        elif (100-sum55)%10 == 8:
            x5=4
        elif (100-sum55)%10 == 1:
            x5=5
        elif (100-sum55)%10 == 3:
            x5=6
        elif (100-sum55)%10 == 5:
            x5=7
        elif (100-sum55)%10 == 7:
            x5=8
        elif (100-sum55)%10 == 9:
            x5=9
        
        wwu = u[0:5]
        Wwu = u[6:]
        print str(wwu) +  str(x5) + str(Wwu)

    elif t == 7 :

        sumU = int (u[0]) + int(u[2]) + int (u[4] )
        V= int (u[1])*2
        sumV= ((V%10) + V / 10)
        Y= int (u[3])*2
        sumY = ((Y%10) + Y / 10)
        Z = int(u[5])*2
        sumZ =((Z%10) + Z /10)

        sum77 = sumU + sumV + sumY + sumZ
        x7= abs(100-sum77)%10
        QQu = u[0:7]
        
        print str(QQu) +  str(x7)
    
