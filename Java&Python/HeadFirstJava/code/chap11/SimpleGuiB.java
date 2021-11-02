import javax.swing.*;
import java.awt.event.*;

public class SimpleGuiB implements ActionListener{
    JButton button;
    public static void main(String[] args){
        SimpleGuiB gui = new SimpleGuiB();
        gui.go();
    }
    public void go(){
        JFrame frame = new JFrame();
        button = new JButton("click me");
        button.addActionListener(this);

        frame.getContentPane().add(button);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(300,200);
        frame.setVisible(true);
    }
    public void actionPerformed(ActionEvent event){
        button.setText("clicked");
    }
}