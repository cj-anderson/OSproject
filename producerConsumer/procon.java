package producerConsumer;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

class ProducerConsumer 
{
    static List<Integer> buffer = new ArrayList<>();
    static int bufferSize = 5;

    static class ProducerThread extends Thread 
    {
        @Override
        public void run() 
        {
            while (true) 
            {
                synchronized (buffer) 
                {
                    if (buffer.size() < bufferSize) 
                    {
                        int item = new Random().nextInt(100) + 1;
                        buffer.add(item);
                        System.out.println("Producer produced: " + item);
                    }
                }
                try 
                {
                    Thread.sleep(new Random().nextInt(5000) + 1000);
                } catch (InterruptedException e) 
                {
                    e.printStackTrace();
                }
            }
        }
    }

    static class ConsumerThread extends Thread 
    {
        @Override
        public void run() 
        {
            while (true) 
            {
                synchronized (buffer) 
                {
                    if (!buffer.isEmpty()) 
                    {
                        int item = buffer.remove(0);
                        System.out.println("Consumer consumed: " + item);
                    }
                }
                try 
                {
                    Thread.sleep(new Random().nextInt(5000) + 1000);
                } catch (InterruptedException e) 
                {
                    e.printStackTrace();
                }
            }
        }
    }

    public static void main(String[] args) 
    {
        for (int i = 0; i < 4; i++) 
        {
            new ProducerThread().start();
        }
        for (int i = 0; i < 2; i++) 
        {
            new ConsumerThread().start();
        }
        new ProducerThread().start();
        new ConsumerThread().start();
    }
}
