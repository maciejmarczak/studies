package org.maciejmarczak.studies.tw.lab9;

import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class FullLockList extends NodeList {

	private Lock lock = new ReentrantLock();
	private final Condition okToRead = lock.newCondition();
	private final Condition okToWrite = lock.newCondition();

	@Override
	boolean contains(Object o) {
		lock.lock();

		try {
			while ((aw + ww) > 0) {
				wr++;
				okToRead.await();
				wr--;
			}
			ar++;
		} catch (InterruptedException e) {
			e.printStackTrace();
		} finally {
			lock.unlock();
		}

		Node tmp = head;
		boolean flag = false;

		while (tmp != null && !flag) {
			if (tmp.o == o)
				flag = true;
			tmp = tmp.next;
		}

		lock.lock();

		try {
			ar--;
			if (ar == 0 && ww > 0)
				okToWrite.signalAll();
			return flag;
		} finally {
			lock.unlock();
		}
	}

	@Override
	boolean remove(Object o) {
		lock.lock();

		try {
			while ((aw + ar) > 0) {
				ww++;
				okToWrite.await();
				ww--;
			}
			aw++;
		} catch (InterruptedException e) {
			e.printStackTrace();
		} finally {
			lock.unlock();
		}

		Node tmp = head, prev = null;
		boolean flag = false;

		if (!(tmp == null)) {
			while (tmp != null && tmp.o != o) {
				prev = tmp;
				tmp = tmp.next;
			}

			if (prev != null && tmp != null) {
				prev.next = tmp.next;
				flag = true;
			} else if (prev == null && tmp != null) {
				head = tmp.next;
				flag = true;
			}
		}

		lock.lock();
		try {
			aw--;

			if (ww > 0)
				okToWrite.signalAll();
			else if (wr > 0)
				okToRead.signalAll();

			return flag;
		} finally {
			lock.unlock();
		}
	}

	@Override
	boolean add(Object o) {
		lock.lock();

		try {
			while ((aw + ar) > 0) {
				ww++;
				okToWrite.await();
				ww--;
			}
			aw++;
		} catch (InterruptedException e) {
			e.printStackTrace();
		} finally {
			lock.unlock();
		}

		Node node = new Node();
		node.o = o;
		node.next = head;
		head = node;

		lock.lock();
		try {
			aw--;

			if (ww > 0)
				okToWrite.signalAll();
			else if (wr > 0)
				okToRead.signalAll();

			return true;
		} finally {
			lock.unlock();
		}
	}

}
