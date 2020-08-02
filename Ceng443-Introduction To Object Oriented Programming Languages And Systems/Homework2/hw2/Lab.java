package hw2;

import java.util.concurrent.*;
import java.util.concurrent.locks.ReentrantLock;

public class Lab {
    private String name;
    private int capacity;

    private Semaphore capacity_semaphore; // there should be no student more than capacity
    private Semaphore isReserved = new Semaphore(1); // isReserved is 1 if there is no a studying group in the lab
    private ReentrantLock rlock = new ReentrantLock(); // It is for writing to countStudent and reading from countStudent

    private StudyGroup reservedStudyGroup = null; // It is saves the StudyGroup which reserves lab (since another group cannot study)
    private int countStudent = 0; // It saves count of studying students in lab

    public Lab(String name, int capacity) {
        this.name = name;
        this.capacity = capacity;
        this.capacity_semaphore = new Semaphore(capacity);
    }

    public String getName() {
        return name;
    }

    public int getCapacity() {
        return capacity;
    }

    public void startStudying(StudyGroup studyGroup) throws InterruptedException {
        
        if (this.reservedStudyGroup != studyGroup) { 
            // if studyGroup is not the study group which was already started to study in this lab, 
            // it will wait to happen the countStundents == 0 (all reservedStudyingGroup students should stop studying)
            isReserved.acquire(); 
            this.reservedStudyGroup = studyGroup;
        }
        this.capacity_semaphore.acquire();
        this.rlock.lock(); // this provides that only one thread can write or read the countStudent
        this.countStudent++;
        this.rlock.unlock();
        
    }

    public void stopStudying() throws InterruptedException {
        
        
        this.rlock.lock();// this provides that only one thread can write or read the countStudent
        this.countStudent--;
        if(this.countStudent == 0){
            this.isReserved.release();
            this.reservedStudyGroup = null;
        }

        this.rlock.unlock();

        this.capacity_semaphore.release();

    }
}
