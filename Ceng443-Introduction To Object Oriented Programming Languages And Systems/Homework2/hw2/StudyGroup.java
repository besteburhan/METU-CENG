package hw2;


public class StudyGroup {
    private String name;
    private Lab preferred_lab;

    public StudyGroup(String name, Lab lab) {
        this.name = name;
        this.preferred_lab = lab;
    }

    public String getName() {
        return name;
    }

    public Lab getLab() {
        return this.preferred_lab;
    }

    public void startStudyingWith() {
        try {
            this.preferred_lab.startStudying(this);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

    }

    public void stopStudyingWith() {
        try {
            this.preferred_lab.stopStudying();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        
    }
}