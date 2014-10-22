function u32(x) {
	return x >>> 0;
}

var mul = Math.imul ? Math.imul : function (a, b) {
	var a1 = a >>> 16, a2 = a & 0xffff;
	var b1 = b >>> 16, b2 = b & 0xffff;
	return (((a1 * b2 + a2 * b1) << 16) + a2 * b2) | 0;
}

function seed_make_const(n) {
	var A = 0x41c64e6d, B = 0x6073;
	var a = A, b = B;
	var c = 1, d = 0;
	while (n) {
		if (n & 1) {
			d = u32(mul(d, a) + b);
			c = mul(c, a);
		}
		b = u32(mul(b, a) + b);
		a = mul(a, a);
		n >>>= 1;
	}
	return { a: c, b: d };
}

function seed_stepper(n) {
	var c = seed_make_const(n);
	return function (seed) {
		return u32(mul(seed, c.a) + c.b);
	};
}

seed_step.cache = [];
(function () {
for (var i = 0; i < 256; i ++) {
	seed_step.cache[i] = seed_make_const(i);
}
})();

function seed_step(seed, n) {
	var c;
	if (0 <= n && n < 256) {
		c = seed_step.cache[n];
	} else {
		c = seed_make_const(n);
	}
	return u32(mul(seed, c.a) + c.b);
}

var seed_next = seed_stepper(1);
var seed_prev = seed_stepper(-1);

function LCG(seed) {
	this.seed = seed;
}

LCG.prototype.setSeed = function (seed) {
	this.seed = seed;
};

LCG.prototype.rand = function () {
	this.seed = seed_next(this.seed);
	return this.seed >>> 16;
};

LCG.prototype.step = function (n) {
	this.seed = seed_step(this.seed, n);
}

function hex(x) {
	return (0x100000000 + x).toString(16).slice(1);
}
