
/* Implementation of weierstrass curve (y^2 = x^3 + ax) over Ring Z/Zn  in Affine coordinate  */
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

struct elliptic_curve_t {
	mpz_t a;
	mpz_t n;
};

struct point_t {
	mpz_t x;
	mpz_t y;
};

struct elliptic_curve_t EC;

void point_doubling(struct point_t P, struct point_t *R)
{
	mpz_t slope, temp;
	mpz_init(temp);
	mpz_init(slope);

	if(mpz_cmp_ui(P.y, 0) != 0) {
		mpz_mul_ui(temp, P.y, 2);
		mpz_invert(temp, temp, EC.n);
		mpz_mul(slope, P.x, P.x);
		mpz_mul_ui(slope, slope, 3);
		mpz_add(slope, slope, EC.a);
		mpz_mul(slope, slope, temp);
		mpz_mod(slope, slope, EC.n);
		mpz_mul(R->x, slope, slope);
		mpz_sub(R->x, R->x, P.x);
		mpz_sub(R->x, R->x, P.x);
		mpz_mod(R->x, R->x, EC.n);
		mpz_sub(temp, P.x, R->x);
		mpz_mul(R->y, slope, temp);
		mpz_sub(R->y, R->y, P.y);
		mpz_mod(R->y, R->y, EC.n);
	} else {
		mpz_set_ui(R->x, 0);
		mpz_set_ui(R->y, 0);
	}
	mpz_clear(temp);
	mpz_clear(slope);
}

void point_addition(struct point_t P, struct point_t Q, struct point_t *R)
{
	mpz_mod(P.x, P.x, EC.n);
	mpz_mod(P.y, P.y, EC.n);
	mpz_mod(Q.x, Q.x, EC.n);
	mpz_mod(Q.y, Q.y, EC.n);

	if(mpz_cmp_ui(P.x, 0) == 0 && mpz_cmp_ui(P.y, 0) == 0) {
		mpz_set(R->x, Q.x);
		mpz_set(R->y, Q.y);
		return;
	}

	if(mpz_cmp_ui(Q.x, 0) == 0 && mpz_cmp_ui(Q.y, 0) == 0) {
		mpz_set(R->x, P.x);
		mpz_set(R->y, P.y);
		return;
	}

	mpz_t temp;
	mpz_init(temp);

	if(mpz_cmp_ui(Q.y, 0) != 0) { 
		mpz_sub(temp, EC.n, Q.y);
		mpz_mod(temp, temp, EC.n);
	} else
		mpz_set_ui(temp, 0);


	if(mpz_cmp(P.y, temp) == 0 && mpz_cmp(P.x, Q.x) == 0) {
		mpz_set_ui(R->x, 0);
		mpz_set_ui(R->y, 0);
		mpz_clear(temp);
		return;
	}

	if(mpz_cmp(P.x, Q.x) == 0 && mpz_cmp(P.y, Q.y) == 0)	{
		point_doubling(P, R);

		mpz_clear(temp);
		return;		
	} else {
		mpz_t slope;
		mpz_init_set_ui(slope, 0);

		mpz_sub(temp, P.x, Q.x);
		mpz_mod(temp, temp, EC.n);
		mpz_invert(temp, temp, EC.n);
		mpz_sub(slope, P.y, Q.y);
		mpz_mul(slope, slope, temp);
		mpz_mod(slope, slope, EC.n);
		mpz_mul(R->x, slope, slope);
		mpz_sub(R->x, R->x, P.x);
		mpz_sub(R->x, R->x, Q.x);
		mpz_mod(R->x, R->x, EC.n);
		mpz_sub(temp, P.x, R->x);
		mpz_mul(R->y, slope, temp);
		mpz_sub(R->y, R->y, P.y);
		mpz_mod(R->y, R->y, EC.n);

		mpz_clear(temp);
		mpz_clear(slope);
		return;
	}
}

void scalar_multiplication( struct point_t *R,struct point_t P, mpz_t m)
{
	struct point_t Q, T;
	mpz_init(Q.x); mpz_init(Q.y);
	mpz_init(T.x); mpz_init(T.y);
	long no_of_bits, loop;

	no_of_bits = mpz_sizeinbase(m, 2);
	mpz_set_ui(R->x, 0);
	mpz_set_ui(R->y, 0);
	if(mpz_cmp_ui(m, 0) == 0)
		return;

	mpz_set(Q.x, P.x);
	mpz_set(Q.y, P.y);
	if(mpz_tstbit(m, 0) == 1){
		mpz_set(R->x, P.x);
		mpz_set(R->y, P.y);
	}

	for(loop = 1; loop < no_of_bits; loop++) {
		mpz_set_ui(T.x, 0);
		mpz_set_ui(T.y, 0);
		point_doubling(Q, &T);

		mpz_set(Q.x, T.x);
		mpz_set(Q.y, T.y);
		mpz_set(T.x, R->x);
		mpz_set(T.y, R->y);
		if(mpz_tstbit(m, loop))
			point_addition(T, Q, R);
	}

	mpz_clear(Q.x); mpz_clear(Q.y);
	mpz_clear(T.x); mpz_clear(T.y);
}		

int main(int argc, char *argv[])
{
	mpz_init(EC.a); 
	mpz_init(EC.n);

	struct point_t p, c,p2;
	mpz_init_set_ui(c.x, 0);
	mpz_init_set_ui(c.y, 0);
	mpz_init_set_ui(p2.x, 0);
	mpz_init_set_ui(p2.y, 0);
	mpz_init(p.x);
	mpz_init(p.y);

	mpz_t e;
	mpz_init(e);
	mpz_t d;
	mpz_init(d);

	mpz_set_str(EC.n, "123299326422097166611346566111902541908141691296831160827882840117916951033595681386632171228337450533830493066659177487916552663604844737288823787090800185444386111224155250857877409885799161965186333743864487006084737422398340733112921245220795627554723973611970151247268321824964350219785551499358224150418349520374929913697725851541536273591030704942877016921209842672914029634038712309167015440922399854304270713692807914084726965849194271541925219398549886687448903361732446177720781225475768466592864628956958693953267077527339236691932786147089937806878460895615797679202090013498068001962479164867889549643596235813358718198137461301172151239980653748227450203123216782155491871569828290150974673218111763842362479437732234874857600551703622809627103298320055006507195833241374797500772450099487824951315093320840761302582368350643725763748746639021678580580951820415494944664765082818519077306332416582164910752014577904129341429378299443986026441937100629095558852167368232371881717224241930436789126997229616561342161276002670105661777428905374290577619009687398014026212469069871806936578907306877696136828134096833697526852408620317734648325558002156783980178136104723247198749193283182925216145170614946556257163615117", 0);

	mpz_set_str(EC.a, "0xc1c627e1638fdc8e24299bb041e4e23af4bb5424", 0);

	mpz_set_str(p.x, "34798323276904195540591952932749320439928080249223614041727757131845193886948597903883744178884051887753955896326307176320326987375725384709369455468269041442088824012191547640830319887848610979174693218048147596457103289107001104469409264624706859340218698376193599219012144393064772721817478803768492925772617329539283643549122330618723311190538454741279449170850685401881709140199638050084094555946885804257318020918693951950184508852223562844955270001934971491755285685847475839185639262988688863874625862439797522746438075393983794221538034742482058488732064039834465082655337336050476456992016445783030157601890304804714161219249728837942921416336493525801029620202717041940366209470334617794960751069738383534921197116935006066933675388129778184990058432522067565859559754909482979285302222547127824889233605901116716595002489072906343656961118705340186687591416964316735266860560256143700058735440907081685434627902829312930510823680170563477217805788434953136434686914422988344187163153471277972528678133900957135688108530834208478793519964331816769646485477890081653086294689609716952373177571790672173826534875845768055242791606413136580144399189232366612331873465024908558526470982141286296728452398345746146083270053029", 0);

	mpz_set_str(p.y, "104598997216216079772449078008695973143328166887517023322064281122526979851276283588036788131213146405567816003856102833484877568607737081262294241378702374776579817255018591923980454060811115214017437247979078212355468685926492174699057996067301624586807724786776289297857067090233347803620497346172046583598450637073933800499872815668458118252461410884296872533073506586252813344193059609993175611499332559423451568583347486757901309332318991638634119682733580862491926576306873547050740541536183589063743974526972409784382332852641676109997411277452721163133555927865028376725755799032807615854860687507463160044746010885224762566286416151009539398721364834228764242822074649681544521017978189521303487328986954289981705654828979870877253289469985910751078122866758681370384585249034069967498826630788477912028578001737127027849789754896272841491444190181233100175902590227023162430564048112328430934078649058992632676787609958187166705564980475573287319076818191088934604135900550938328223931736602753421767437081255258548490913801477473237212034273208584030366775752979249827565038124357666426557792589228758334384272042404624550514728838073710335697359509211290916938194817489861300877124115893629666377311946016896308091228633", 0);

	mpz_set_str(e, "74564220401741518971542485985189651631384381573996938523297958225239450320387", 0);
	mpz_set_str(d, "65460657701508410723853454496633855616564205243218342547150653911235322151850379009298133001231450126244910035517525668473085075376966725249162020394804808900521646979337321818938986112772670223227115420583690748518950312572341181535404937013499407282935410504149874346202199396174638938202664131107755963399017236831571598989888783826467657024678207779257795532166882561023982457227543257640034858234889196579115354291252175560334639367808130898310716346553593647218219349873800051000094300479450929199089196243635501239174175623841125216153947033971736642730395761492142029361957698030364194972423876598879407380207118818757498504485536601675258585648807882640817049152078730828805150015710166767841201788132166863598872952524771273987493130941010347258423002386099976770930977266951754877516000245931482434853381643029220096819395768918314564767925682917889805861070294666026550623417999633929778615663969582403982285735156097418628849372251911524539339967639579606395675210712476341265113235418820135574605272730663385904176196555231893189982629123929013614216298295016559362571854567628713245613977960510993862420101824501798669089243647460039470778033566213812880541912360914353738724868482638408030501788477336733659615658027", 0);

	printf("\nplain text:\n--------------\n");	
	printf("\np.x: ");	
	mpz_out_str(stdout, 16, p.x); puts("");
	printf("\np.y: ");	
	mpz_out_str(stdout, 16, p.y); puts("");

	scalar_multiplication(&c,p, e);

	printf("\ncipher text:\n--------------\n");	
	printf("\nc.x: ");	
	mpz_out_str(stdout, 16, c.x); puts("");
	printf("\nc.y: ");	
	mpz_out_str(stdout, 16, c.y); puts("");
	
	scalar_multiplication(&p2,c, d);

	printf("\nplain text after decryption:\n----------------------\n");	
	printf("\np2.x: ");	
	mpz_out_str(stdout, 16, p2.x); puts("");
	printf("\np2.y: ");	
	mpz_out_str(stdout, 16, p2.y); puts("");



	// Free variables
	mpz_clear(EC.a);  mpz_clear(EC.n);
	mpz_clear(c.x); mpz_clear(c.y);
	mpz_clear(p.x); mpz_clear(p.y);
	mpz_clear(e);
	mpz_clear(d);
}
