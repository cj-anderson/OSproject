package producerConsumer;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class procon {

    static List<Integer> buffer = new ArrayList<>();
    static int bufferSize = 5;

    static class ProducerThread extends Thread {
        @Override
        public void run() {
            while (true) {
                synchronized (buffer) {
                    if (buffer.size() < bufferSize) {
                        int item = new Random().nextInt(100) + 1;
                        buffer.add(item);
                        System.out.println("Producer produced: " + item);
                    }
                }
                try {
                    Thread.sleep(new Random().nextInt(5000) + 1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    static class ConsumerThread extends Thread {
        @Override
        public void run() {
            while (true) {
                synchronized (buffer) {
                    if (!buffer.isEmpty()) {
                        int item = buffer.remove(0);
                        System.out.println("Consumer consumed: " + item);
                    }
                }
                try {
                    Thread.sleep(new Random().nextInt(5000) + 1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public static void main(String[] args) {
        String filePath = "producerConsumer/datafile.txt"; //Relative path to file
        
        // Get the current working directory
        String currentDirectory = System.getProperty("user.dir");
        String absoluteFilePath = currentDirectory + File.separator + filePath;

        try (BufferedReader reader = new BufferedReader(new FileReader(absoluteFilePath))) {
            String line;
            while ((line = reader.readLine()) != null) {
                String[] parts = line.trim().split("\\s+");
                int numProducers = Integer.parseInt(parts[0]);
                int numConsumers = Integer.parseInt(parts[1]);
                for (int i = 0; i < numProducers; i++) {
                    new ProducerThread().start();
                }
                for (int i = 0; i < numConsumers; i++) {
                    new ConsumerThread().start();
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

