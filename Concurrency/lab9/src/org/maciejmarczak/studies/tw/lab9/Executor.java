package org.maciejmarczak.studies.tw.lab9;

import java.util.Random;

class Executor {
	private NodeList list;
	private Object[] objects;
	private int n, writers, readers;
	private Actor[] w, r;
	final Random rand = new Random();
	
	Executor(NodeList list, int n, int writers, int readers) {
		this.list = list;
		this.n = n;
		this.writers = writers;
		this.readers = readers;
		objects = new Object[n];
		w = new Actor[writers];
		r = new Actor[readers];
	}
	
	void start() {
		for (int i = 0; i < n; i++) {
			objects[i] = new Object();
			list.add(objects[i]);
		}
		
		for (int i = 0; i < readers; i++) {
			r[i] = ActorFactory.getActor(list, objects[rand.nextInt(n - 1)], Role.READER, n, i);
		}
		
		for (int i = 0; i < writers; i++) {
			w[i] = ActorFactory.getActor(list, objects[rand.nextInt(n - 1)],
					(i % 2 == 0 ? Role.WRITER_ADD : Role.WRITER_REMOVE), n * 10, i);
		}
		
		for (int i = 0; i < Math.max(readers, writers); i++) {
			if ( i < readers ) r[i].start();
			if ( i < writers ) w[i].start();
		}
		
		try {
			for (int i = 0; i < Math.max(readers, writers); i++) {
				if ( i < readers ) r[i].join();
				if ( i < writers ) w[i].join();
			}
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
}
