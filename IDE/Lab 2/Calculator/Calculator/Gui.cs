using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Calculator
{
    public partial class Gui : Form
    {
        Core core = new Core();

        public Gui()
        {
            InitializeComponent();
        }

        private void button_Click(object sender, EventArgs e)
        {
            if (result.Text == "0" || core.getbutpressed())
               result.Clear();

            Button b = (Button)sender;

            core.setbutpressed(false);

            if (b.Text == ".")
            {
                if (!result.Text.Contains("."))
                    core.setresult(result.Text + b.Text);
                    result.Text = core.getresult();
            }
            else {
                core.setresult(result.Text + b.Text);
                result.Text = core.getresult();
            }
            

        }

        private void opertor_Click(object sender, EventArgs e)
        {
            Button b = (Button)sender;
 
                core.setoperation(b.Text);
                core.setoresult(result.Text);
                core.setbutpressed(true);
            if (core.getoperation() == "sqrt")
            {
                equation.Text = core.getoperation() + " " + core.getoresult();
            }
            else {
                equation.Text = core.getoresult() + " " + core.getoperation();
            }
            

        }

        private void equal_Click(object sender, EventArgs e)
        {
            core.setequation("");
            equation.Text = core.getequation();
            core.Equal();
            result.Text = core.getvalue().ToString();
            core.setbutpressed(false);
        }

        private void clear_Click(object sender, EventArgs e)
        {
            equation.Text = "";
            result.Clear();
            result.Text = "0";
        }


        private void signbut(object sender, EventArgs e)
        {
            result.Text = core.sign(result.Text);

        }
    }
}
