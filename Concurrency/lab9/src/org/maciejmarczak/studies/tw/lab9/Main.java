package org.maciejmarczak.studies.tw.lab9;

public class Main {
	final static boolean OUTPUT = false;
	
	public static void main(String[] args) {
		Executor ex;
		long start;
		float elapsed_full, elapsed_grained;
		
		for ( int i = 1; i <= 10; i++ ) {
			for (int j = 10; j <= 100; j++ ) {
				start = System.currentTimeMillis();
				
				ex = new Executor(new FullLockList(), 100, i, j);
				ex.start();
				
				elapsed_full = (System.currentTimeMillis() - start)/1000F;
				
				start = System.currentTimeMillis();
				
				ex = new Executor(new FullLockList(), 100, i, j);
				ex.start();
				
				elapsed_grained = (System.currentTimeMillis() - start)/1000F;
				
				System.out.println(i + " " + j + " " + elapsed_full + " " + elapsed_grained);
			}
		}
	}
}
