input metauml;

beginfig(1);
	Begin.rb;
	End.re;

	State.ascending("Ascending")();
	State.descending("Descending")();
	State.cooling("Cooling")();

	State.running("Running")(rb, re, ascending, descending, cooling);
	ascending.midx = rb.midx;
	ascending.top = rb.bottom - 20;

	descending.midx = ascending.midx;
	descending.top = ascending.bottom - 20;

	cooling.midy = ascending.midy;
	cooling.left = ascending.right + 20;

	re.midx = cooling.midx;
	re.midy = descending.midy;

	running.info.drawNameLine := 1;
	drawObject(running);
	link(transition)(rb.s -- ascending.n);
	draw image(link(transition)(ascending.s -- descending.n)) shifted(-5,0);
	draw image(link(transition)(descending.n -- ascending.s)) shifted(5,0);
	link(transition)(ascending.e -- cooling.w);
	link(transition)(descending.ne -- cooling.sw);
	link(transition)(cooling.s -- re.n);

	Begin.b;
	End.e;
	State.idle("Idle")();

	idle.midy = running.midy;
	idle.right = running.left - 40;

	b.midx = idle.midx;
	b.bottom = idle.top + 30;

	e.midx = idle.midx;
	e.top = idle.bottom - 30;

	drawObject(idle);
	drawObject(b);
	drawObject(e);

	link(transition)(b.s -- idle.n);
	link(transition)(idle.s -- e.n);
	draw image(link(transition)(idle.e -- running.w)) shifted(0,5);
	draw image(link(transition)(running.w -- idle.e)) shifted(0,-5);
	link(transition)((running.left,e.midy) -- e.e);
	link(transition)(rpathHorizontal(e.e, running.left));
endfig;

end;

