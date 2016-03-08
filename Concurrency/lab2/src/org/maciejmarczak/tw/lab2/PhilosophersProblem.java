package org.maciejmarczak.tw.lab2;

public class PhilosophersProblem {
    public static void main(String[] args) {
        Philosopher[] philosophers = new Philosopher[5];

        for (int i = 0; i < 5; i++) {
            philosophers[i] = new Philosopher();
            philosophers[i].start();
        }

        // 2nd solution
        // new PhilosophersWatcher(philosophers).start();
    }
}

class PhilosophersWatcher extends Thread {
    Philosopher[] philosophers;

    PhilosophersWatcher(Philosopher[] philosophers) {
        this.philosophers = philosophers;
    }

    @Override
    public void run() {
        int leftForks, rightForks;

        while (true) {
            leftForks = rightForks = 0;

            for (int i = 0; i < 5; i++) {
                if (philosophers[i].holdsLeftFork) leftForks++;
                if (philosophers[i].holdsRightFork) rightForks++;
            }

            if (leftForks == 5 || rightForks == 5) {
                resetTable();
            }
        }
    }

    private void resetTable() {
        for (int i = 0; i < 5; i++) {
            philosophers[i].holdsRightFork = philosophers[i].holdsLeftFork = false;
            Philosopher.table.forks[i].release();
        }
    }
}

class Table {
    int forksTaken = 0;
    Fork[] forks = new Fork[5];
    CustomSemaphore semaphore = new CustomSemaphore(1);

    Table() {
        for (int i = 0; i < forks.length; i++) {
            forks[i] = new Fork();
        }
    }

    class Fork {
        boolean isTaken = false;

        void take() {
            isTaken = true;
            forksTaken++;
        }

        void release() {
            isTaken = false;
            forksTaken--;
        }
    }
}

class Philosopher extends Thread {
    private static int numOfPhilosophers = 0;
    private int id;

    boolean holdsLeftFork, holdsRightFork;

    static Table table = new Table();
    Table.Fork left, right;

    Philosopher() {
        id = ++numOfPhilosophers;
        left = table.forks[id - 1];
        right = table.forks[id % 5];
    }

    @Override
    public void run() {
        while (true) {
            try {
                Thread.sleep(5);

                if (Math.random() > 0.5) {
                    tryGettingFork();
                }

                if (Math.random() > 0.5) {
                    tryReleasingFork();
                }

            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    private void tryGettingFork() {

        // 1st solution
        /*if (table.forksTaken == 4 && !holdsRightFork && !holdsLeftFork) {
            return;
        }*/

        /*if (!holdsRightFork && !right.isTaken) {
            right.take();
            holdsRightFork = true;
            System.out.println(this);
        }

        if (!holdsLeftFork && !left.isTaken) {
            left.take();
            holdsLeftFork = true;
            System.out.println(this);
        }*/

        // 3rd solution

        CustomSemaphore semaphore = Philosopher.table.semaphore;

        try {
            semaphore.acquire();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        if (!holdsRightFork && !right.isTaken && !holdsLeftFork && !left.isTaken) {
            right.take();
            holdsRightFork = true;

            left.take();
            holdsLeftFork = true;

            System.out.println(this);
        }
        semaphore.release();

    }

    private void tryReleasingFork() {

        if (holdsRightFork && holdsLeftFork) {

            right.release();
            holdsRightFork = false;

            left.release();
            holdsLeftFork = false;

            System.out.println(this);
        }

    }

    @Override
    public String toString() {
        return "ID: " + id + " " + (holdsLeftFork ? "LEFT " : "") + (holdsRightFork ? "RIGHT" : "");
    }
}

class CustomSemaphore {
    private int permits;

    CustomSemaphore(int permits) {
        this.permits = permits;
    }

    synchronized void acquire() throws InterruptedException {

        if (permits > 0) {
            permits--;
        } else {
            while (permits < 1) {
                this.wait();
            }
            permits++;
        }

    }

    synchronized void release() {
        permits++;

        if (permits > 0) {
            this.notifyAll();
        }
    }
}