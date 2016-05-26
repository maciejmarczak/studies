package org.maciejmarczak.studies.tw.lab9;

import java.util.function.Function;

class Actor extends Thread {

	private Object o;
	private Function<Object, Boolean> f;
	private int n;
	private String success, failure;
	
	Actor(Object o, Function<Object, Boolean> f, int n, String success, String failure) {
		this.o = o;
		this.f = f;
		this.n = n;
		this.success = success;
		this.failure = failure;
	}
	
	@Override
	public void run() {
		for (int i = 0; i < n; i++) {
			boolean res = f.apply(o);
			
			if (Main.OUTPUT) {
				if (res) {
					System.out.println(success);
				} else {
					System.out.println(failure);
				} 
			}
			
			/*try {
				Thread.sleep((long)(Math.random() * 300));
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}*/
		}
	}
	
}
