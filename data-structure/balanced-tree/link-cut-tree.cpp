void access (int x) {
	int u = x, v = -1;
	while (u != -1) {
		splay (u); push_down (u);
		if (~n[u].c[1]) n[n[u].c[1]].f = -1, n[n[u].c[1]].p = u;
		n[u].c[1] = v;
		if (~v) n[v].f = u, n[v].p = -1;
		update (u); u = n[v = u].p; }
	splay (x); }
