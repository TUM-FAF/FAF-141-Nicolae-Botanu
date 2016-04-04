using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Calculator
{
    class Core
    {
        private string result = "";
        private string result2 = "";
        private string operation = "";
        private string equation = "";
        private bool butpres = false;
        private double value = 0;
        public void setresult(string r)
        {
            this.result = r;
        }

        public string getresult()
        {
            return this.result;
        }
        public void setoresult(string r)
        {
            this.result2 = r;
        }

        public string getoresult()
        {
            return this.result2;
        }
        public void setequation(string r)
        {
            this.equation = r;
        }

        public string getequation()
        {
            return this.equation;
        }

        public void setoperation(string r)
        {
            this.operation = r;
        }

        public string getoperation()
        {
            return this.operation;
        }

        public void setvalue(double v)
        {
            this.value = v;
        }

        public double getvalue()
        {
            return this.value;
        }

        public void setbutpressed(bool b)
        {
            this.butpres = b;
        }

        public bool getbutpressed()
        {
            return this.butpres;
        }

        public string sign(string result)
        {
            if (result.StartsWith("-"))
            {
                result = result.Substring(1);
                return result;
            }
            else if (!string.IsNullOrEmpty(result) && decimal.Parse(result) != 0)
            {
                result = "-" + result;
                return result;
            }
            else
            {
                return result;
            }
        }

        public void Equal()
        {
            switch (operation)
            {

                case "+":

                    value = Double.Parse(this.result2) + Double.Parse(this.result);
                    break;
                case "-":
                    value = Double.Parse(this.result2) - Double.Parse(this.result);
                    break;
                case "/":
                    value = Double.Parse(this.result2) / Double.Parse(this.result);
                    break;
                case "*":
                    value = Double.Parse(this.result2) * Double.Parse(this.result);
                    break;
                case "^":
                    value = Math.Pow(Double.Parse(this.result2), Double.Parse(this.result));
                    break;
                case "sqrt":
                    value = Math.Sqrt(Double.Parse(this.result2));
                    break;
                default:
                    break;
            }
            butpres = false;
        }

    }
}
