# This file show the implementation of KMOV scheme, HAL @02-02-2020


from sage.all import *

def searchingForUandV(l):
    u=randint(1 ,2**l)
    while (u%4 != 3):
        u=randint(1,2**l)

    v = randint(1, 2 ** l)

    while (v % 2 != 0):
        v = randint(1, 2 ** l)

    return u,v

def KeyGeneration(l):
    print "KeyGeneration"
    up,vp=searchingForUandV(l)
    p = up ** 2 + vp ** 2

    while (is_prime(p)==False):
        up, vp = searchingForUandV(l)
        p = up ** 2 + vp ** 2

    # print "p=", p
    # print "up=", up
    # print "up%4=", up % 4
    # print "u is odd:", is_odd(up)
    # print "v=", vp
    # print "v is even:", is_even(vp)

    uq, vq = searchingForUandV(l)
    q = uq ** 2 + vq ** 2

    while (is_prime(q)==False):
        uq, vq = searchingForUandV(l)
        q = uq ** 2 + vq ** 2

 #   print "q=", q
    n=p*q
    R=IntegerModRing(n)
 #   print "n=",n
    e = randint(1, 2**256)

#    while( gcd(e,n) != 1):
#        e = randint(1, 2 ** 256)

#    while( gcd(e,R((p**2 -4*up**2)*(q**2 -4*uq**2))) != 1):
#        e = randint(1, 2 ** 256)
    while( gcd(e,( p + 1 + 2 * up)*( p + 1 - 2 * up)*(p + 1 + 2 * vp)* (p + 1 - 2 * vp)* (q + 1 + 2 * uq) * (q + 1 - 2 * uq)* (q + 1 + 2 * vq)* (q + 1 - 2 * vq)) != 1):
        e = randint(1, 2 ** 256)
  #  print "e=",e
    return p,up,vp,q,uq,vq,n,e



def encrypt(e,n,mx,my):
    R = IntegerModRing(n)
#    print "mx=", R(mx)
#    print "my=", R(my)
    a= R(((my**2 - mx**3) * inverse_mod(int(mx),n)))
#    print "\n\na0=", a
    E=EllipticCurve(R,[a,0])
    g=E(R(mx),R(my))
    return g*e

def decrypt(ctx,cty,n,e,p,up,vp,q,uq,vq):
    R = IntegerModRing(n)
    Rp = IntegerModRing(p)
    Rq = IntegerModRing(q)
    a = R((cty **2 - ctx**3) * inverse_mod(int(ctx), n))
#    print "\na1=",a
    E = EllipticCurve(R, [a, 0])
    g = E(ctx, cty)
    ind=power_mod(a,(p-1)/4,p)
    print "ind p=",ind
#    print "p-1=", p-1
#    print "Rp(up*vp^-1)=", Rp(up* inverse_mod(vp,p))
#    print "Rp(-up*vp^-1)=", Rp(-up * inverse_mod(vp,p))
#    np=0
    if (ind == 1):
        np= p + 1 + 2 * up
    elif(ind == p-1):
        np = p + 1 - 2 * up
    elif(ind ==(Rp(-up*inverse_mod(vp,p)))):
        np = p + 1 + 2 * vp
    elif (ind == (Rp((up * inverse_mod(vp,p))))):
        np = p + 1 - 2 * vp

#    print np==0

    ind=Rq(power_mod(a,(q-1)/4,q))

    print "ind q=",ind
    if (ind == 1):
       nq = q + 1 + 2 * uq
    elif (ind == q - 1):
       nq = q + 1 - 2 * uq
    elif (ind == (Rq(-uq * inverse_mod(vq,q)))):
        nq = q + 1 + 2 * vq
    elif (ind == (Rq((uq * inverse_mod(vq,q))))):
        nq = q + 1 - 2 * vq

    d=inverse_mod(int(e),int(np*nq))

    return d*g

###### TEST #######

l=1023


p= 6113315723928743401759862417887745021195804966133352363509872617825404877584359476296634121447364856226809509935142085481294638079897612606595719499234282939546115253994453708687493020727294433253619087096418691583141835062162856664383026647356846865821518292378305341265973502096353031548563378856733902564884646441047890550421212181136624138486957806380885421852891781574911294013373340285139145894001462334580557602293187413021392441695422653994074775257304940029161251015955185438466585574943519277623978427154599789822447304932274336163593068348344203636045006403442470119319253117064069589569270036200653240917

up= 72798888359373539163312679913677577369007310783358077052122471289911352683924478896634672286251667611630926745724017345426511455509428112846297294347214682544163439793649408864362734585584902944792608575125281676635369312561668384570955888392841523562917345441155185238988420194220781918958310775472978516519

vp= 28524333078412389363053501960383776219355085595777672754620151249406562629458941120649812598093271337450690456928826861414197714865681700664054077885390288349581195002907173734859498739889328469151841088625709860265869473047905688790208368867431012989818113295403132876618022601963596311755573612499362310666

q= 20168977358633528891809145953746039878537276687717177431831899649323887376475672181565327727722967889538243593556607110935588957739723063187682004567929083720336516499349445361606971868414406200745867633555563450547113337752912810775987581225592561834405406422288886525028152443241428253998719723593739796823509514027392594543621677812238628874166662375266341950554316353570541643429055495140839570674759730758807755480696887128303758971529113256154310581530613094176746563277201511750285557096422226029378422407862853549045670565340909091569798290301961580409351878159785104655595882117859425247622684068549809472601

uq= 128904968075974936485853823572267306845135357341991832273189699576778666101617601186898275771201435167152206936991538764516985838465896380509595055660083529537237365365860544936967829608058977053146377671290166802152171756780662240304686651638547387463563422554227012293480092841560630524479238470721098384635

vq= 59602739567618965051451967107699043419048043765860128363159446785340743468360781050645511673905493713428417275420954065385986123610706100846795280882222797663387416230043586602588611021580334941202569325621779251825045855530019275965642996827416215280080037891193156450306185819811452556262525087163499374376

n= 123299326422097166611346566111902541908141691296831160827882840117916951033595681386632171228337450533830493066659177487916552663604844737288823787090800185444386111224155250857877409885799161965186333743864487006084737422398340733112921245220795627554723973611970151247268321824964350219785551499358224150418349520374929913697725851541536273591030704942877016921209842672914029634038712309167015440922399854304270713692807914084726965849194271541925219398549886687448903361732446177720781225475768466592864628956958693953267077527339236691932786147089937806878460895615797679202090013498068001962479164867889549643596235813358718198137461301172151239980653748227450203123216782155491871569828290150974673218111763842362479437732234874857600551703622809627103298320055006507195833241374797500772450099487824951315093320840761302582368350643725763748746639021678580580951820415494944664765082818519077306332416582164910752014577904129341429378299443986026441937100629095558852167368232371881717224241930436789126997229616561342161276002670105661777428905374290577619009687398014026212469069871806936578907306877696136828134096833697526852408620317734648325558002156783980178136104723247198749193283182925216145170614946556257163615117

e=74564220401741518971542485985189651631384381573996938523297958225239450320387
mx=randint(1,n)
my=randint(1,n)

#mx= 34798323276904195540591952932749320439928080249223614041727757131845193886948597903883744178884051887753955896326307176320326987375725384709369455468269041442088824012191547640830319887848610979174693218048147596457103289107001104469409264624706859340218698376193599219012144393064772721817478803768492925772617329539283643549122330618723311190538454741279449170850685401881709140199638050084094555946885804257318020918693951950184508852223562844955270001934971491755285685847475839185639262988688863874625862439797522746438075393983794221538034742482058488732064039834465082655337336050476456992016445783030157601890304804714161219249728837942921416336493525801029620202717041940366209470334617794960751069738383534921197116935006066933675388129778184990058432522067565859559754909482979285302222547127824889233605901116716595002489072906343656961118705340186687591416964316735266860560256143700058735440907081685434627902829312930510823680170563477217805788434953136434686914422988344187163153471277972528678133900957135688108530834208478793519964331816769646485477890081653086294689609716952373177571790672173826534875845768055242791606413136580144399189232366612331873465024908558526470982141286296728452398345746146083270053029
#my= 104598997216216079772449078008695973143328166887517023322064281122526979851276283588036788131213146405567816003856102833484877568607737081262294241378702374776579817255018591923980454060811115214017437247979078212355468685926492174699057996067301624586807724786776289297857067090233347803620497346172046583598450637073933800499872815668458118252461410884296872533073506586252813344193059609993175611499332559423451568583347486757901309332318991638634119682733580862491926576306873547050740541536183589063743974526972409784382332852641676109997411277452721163133555927865028376725755799032807615854860687507463160044746010885224762566286416151009539398721364834228764242822074649681544521017978189521303487328986954289981705654828979870877253289469985910751078122866758681370384585249034069967498826630788477912028578001737127027849789754896272841491444190181233100175902590227023162430564048112328430934078649058992632676787609958187166705564980475573287319076818191088934604135900550938328223931736602753421767437081255258548490913801477473237212034273208584030366775752979249827565038124357666426557792589228758334384272042404624550514728838073710335697359509211290916938194817489861300877124115893629666377311946016896308091228633

#print KeyGeneration(l)


ct=encrypt(e,n,mx,my)

#print ct

pt=decrypt(ct[0],ct[1],n,e,p,up,vp,q,uq,vq)

print "ENC/DEC CHECK:",
print pt[0]==mx,

print pt[1]==my


i=0

while (i<100):
    mx = randint(1, n)
    my = randint(1, n)
    p,up,vp,q,uq,vq,n,e= KeyGeneration(l)
    ct = encrypt(e, n, mx, my)
    pt = decrypt(ct[0], ct[1], n, e, p, up, vp, q, uq, vq)

    print "ENC/DEC CHECK:",i,
    print pt[0] == mx,
    print pt[1] == my
    if (pt[0]!= mx or pt[1]!=my):
        print "Error at ", i
        break
    i+=1


