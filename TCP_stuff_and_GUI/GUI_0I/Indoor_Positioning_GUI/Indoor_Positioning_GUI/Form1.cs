using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using GMap.NET;
using GMap.NET.WindowsForms;
using GMap.NET.WindowsForms.Markers;
using GMap.NET.MapProviders;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace Indoor_Positioning_GUI
{

    public partial class Form1 : Form
    {


        private string lat_text = "";
        private string long_text = "";
        private string latError_text = "";
        private string longError_text = "";
        private string markerID_text = "";
        private bool networkIsDrawn = false;
        private List<MyMarker> marker_List = new List<MyMarker>();
        private GMapOverlay markers = new GMapOverlay("markers");
        private GMapOverlay polygons = new GMapOverlay("polygons");
        private GMapOverlay network = new GMapOverlay("network");
        private DataTable dataTable = new DataTable();
        private BindingSource source = new BindingSource();
        private Thread reCenterThread;
        private Thread socketThread;
        private bool stopSocket = false;
        private bool killRecenter = false;

        #region STARTUP 
        public Form1()
        {
            InitializeComponent();
            this.MinimizeBox = false;
            this.MaximizeBox = false;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            socketThread = new Thread(startSocket);
            socketThread.Start();
        }
        private void webBrowser1_DocumentCompleted(object sender, WebBrowserDocumentCompletedEventArgs e)
        {

        }
        private void gMap_Load(object sender, EventArgs e)
        {


            gMap.MapProvider = GMap.NET.MapProviders.BingMapProvider.Instance;
            GMap.NET.GMaps.Instance.Mode = GMap.NET.AccessMode.ServerOnly;
            gMap.SetPositionByKeywords("University of Calgary, Canada");
            gMap.ShowCenter = false;
            gMap.Overlays.Add(markers);

            dataTable.Columns.Add("ID");
            dataTable.Columns.Add("Lat");
            dataTable.Columns.Add("Long");
            dataTable.Columns.Add("Lat Error");
            dataTable.Columns.Add("Long Error");


            tableGridView.AutoGenerateColumns = true;
            source.DataSource = dataTable;
            tableGridView.DataSource = source;
            tableGridView.RowHeadersVisible = false;
            tableGridView.AllowUserToAddRows = false;
            tableGridView.ReadOnly = true;

            gMap.Overlays.Add(network);
            gMap.Overlays.Add(polygons);            
            gMap.Overlays.Add(markers);


        }
        #endregion 

        #region LABELS
        private void latitude_label_Click(object sender, EventArgs e)
        {
            //
        }

        private void longitude_label_Click(object sender, EventArgs e)
        {
            //
        }

        private void error_label_Click(object sender, EventArgs e)
        {
            //
        }
        private void longerror_label_Click(object sender, EventArgs e)
        {

        }
        #endregion

        #region CHECKBOXES
        private void network_checkBox_CheckedChanged(object sender, EventArgs e)
        {
            drawMarkers();
        }

        private void errorelipses_checkBox_CheckedChanged(object sender, EventArgs e)
        {
            drawMarkers();
        }

        private void latlong_checkBox_CheckedChanged(object sender, EventArgs e)
        {
            drawMarkers();
        }
        #endregion

        #region TEXTBOXES
        private void error_textBox_TextChanged(object sender, EventArgs e)
        {
            latError_text = error_textBox.Text;
        }

        private void error2_textBox_TextChanged(object sender, EventArgs e)
        {
            longError_text = error2_textBox.Text;
        }

        private void longitude_textBox_TextChanged(object sender, EventArgs e)
        {
            long_text = longitude_textBox.Text;
        }

        private void latitude_textBox_TextChanged(object sender, EventArgs e)
        {
            lat_text = latitude_textBox.Text;
        }
        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            markerID_text = textBox1.Text;
        }

        private void tableGridView_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

        }
        #endregion

        #region BUTTONS
        private void generate_button_Click(object sender, EventArgs e)
        {
            updateMarkerList();
            drawMarkers();
            updateTable();
        }
        #endregion

        #region BACKGROUND FUNCTIONS

        private void updateTable()
        {            
            for (int i = 0; i < marker_List.Count(); i++)
            {
                bool updated = false;
                foreach(DataRow rData in dataTable.Rows)
                {
                    String temp = rData["ID"].ToString();
                    if ((temp.Contains(marker_List[i].getMarkerID())))
                    {
                        rData["Lat"] = marker_List[i].getLatitude();
                        rData["Long"] = marker_List[i].getLongitude();
                        rData["Lat Error"] = marker_List[i].getXerror();
                        rData["Long Error"] = marker_List[i].getYerror();
                        updated = true;
                        
                    }
                   
                }

                
                if(!updated)
                {
                    DataRow newRow = dataTable.NewRow();
                    newRow["ID"] = marker_List[i].getMarkerID();
                    newRow["Lat"] = marker_List[i].getLatitude();
                    newRow["Long"] = marker_List[i].getLongitude();
                    newRow["Lat Error"] = marker_List[i].getXerror();
                    newRow["Long Error"] = marker_List[i].getYerror();

                    dataTable.Rows.Add(newRow);
                    
                }
                                   
                    
            }
            source.ResetBindings(false);
        }

        private void draw_ellipse(double lat, double longi, double latError, double longError)
        {
            List<PointLatLng> points = new List<PointLatLng>();
            double seg = Math.PI * 2 / 1000;
            for (int i = 0; i < 1000; i++)
            {
                double theta = seg * i;
                double a = lat + Math.Cos(theta) * longError / 111111;
                double b = longi + Math.Sin(theta) * latError / 111111;
                points.Add(new PointLatLng(a, b));
            }
            GMapPolygon elipse = new GMapPolygon(points, "error elipse");
            
           // gMap.Overlays.Add(polygons);
            polygons.Polygons.Add(elipse);
        }

        private void draw_Network()
        {
            MyMarker averageCenter = getCenter();
            
            gMap.SetPositionByKeywords(averageCenter.getLatitude() +","+averageCenter.getLongitude());

            if (marker_List.Count() != 0)
            {
                List<PointLatLng> points2 = new List<PointLatLng>();
                points2.Add(new PointLatLng(gMap.Position.Lat + 1, gMap.Position.Lng + 1));
                points2.Add(new PointLatLng(gMap.Position.Lat - 1, gMap.Position.Lng + 1));
                points2.Add(new PointLatLng(gMap.Position.Lat - 1, gMap.Position.Lng - 1));
                points2.Add(new PointLatLng(gMap.Position.Lat + 1, gMap.Position.Lng - 1));

                GMapPolygon square2 = new GMapPolygon(points2, "square1");
                square2.Fill = new SolidBrush(Color.FromArgb(225, Color.Gray));
                square2.Stroke = new Pen(Color.Gray, 0);

                //gMap.Overlays.Add(network);
                network.Polygons.Add(square2);
                network.Polygons.Add(square2);
            }
            
            
        }

        private void updateMarkerList()
        {
            string latitude = lat_text;
            string longitude = long_text;
            string latError = latError_text;
            string longError = longError_text;
            string markerID = markerID_text;

            if (longitude == "" || latitude == "" || latError == "" || longError == "")
            {
                return;
            }
            
            if (latitude.Contains("RMV"))
            {
                
                for (int i = 0; i<marker_List.Count(); i++)
                {
                    
                    if (markerID.Contains(marker_List[i].getMarkerID()))
                    {
                        
                        marker_List.RemoveAt(i);
                        
                        for (int j = 0; j < dataTable.Rows.Count; j++)
                        {
                            
                            if ((dataTable.Rows[j]["ID"].ToString()).Contains(marker_List[i].getMarkerID()))
                            {
                                
                                dataTable.Rows.RemoveAt(j);
                                source.ResetBindings(false);
                                return;
                            }
                        }
                        
                    }
                }
            }
            bool checkUpdated = false;
            for (int i = 0; i < marker_List.Count(); i++)
            {
                if (marker_List[i].getMarkerID() == markerID)
                {
                    marker_List[i].update(latitude, longitude, latError, longError);
                    checkUpdated = true;
                    
                }
            }

            if (!checkUpdated)
            {
                MyMarker newMarker = new MyMarker(markerID, latitude, longitude, latError, longError);
                marker_List.Add(newMarker);
            }

            
        }

        private void drawMarkers()
        {

            polygons.Polygons.Clear();
            markers.Markers.Clear();
                        
            
            /*gMap.Overlays.Remove(network);
            gMap.Overlays.Remove(markers);
            gMap.Overlays.Remove(polygons);*/

            if (network_checkBox.Checked)
            {
                
                gMap.MaxZoom = 19;
                gMap.MinZoom = 19;
                gMap.Zoom = 19;
                gMap.CanDragMap = false;                
                if(!networkIsDrawn)
                {
                    reCenterThread = new Thread(reCenter);
                    reCenterThread.Start();
                    gMap.Overlays.Remove(markers);
                    gMap.Overlays.Remove(polygons);
                    draw_Network();
                    gMap.Overlays.Add(polygons);
                    gMap.Overlays.Add(markers);
                    
                    networkIsDrawn = true;
                }
                


            }
            else
            {
                if(reCenterThread != null)
                {
                    if (reCenterThread.IsAlive)
                    {
                        reCenterThread.Abort();
                    }
                }
                
                
                gMap.MaxZoom = 22;
                gMap.MinZoom = 2;
                gMap.CanDragMap = true;
                networkIsDrawn = false;
                network.Polygons.Clear();
            }

            foreach (MyMarker m in marker_List)
            {
                GMap.NET.WindowsForms.GMapMarker marker = new GMarkerGoogle(new GMap.NET.PointLatLng(Convert.ToDouble(m.getLatitude()), Convert.ToDouble(m.getLongitude())), GMarkerGoogleType.blue_small);
                if (latlong_checkBox.Checked)
                {
                    marker.ToolTipText = "ID: " + m.getMarkerID(); //+ "\nLatitude: " + m.getLatitude() + "\nLongitude: " + m.getLongitude() + "\nLat Error: " + m.getXerror() + "\nLong Error: " + m.getYerror();
                    marker.ToolTipMode = MarkerTooltipMode.Always;
                }
                if (!latlong_checkBox.Checked)
                {
                    marker.ToolTipMode = MarkerTooltipMode.Never;
                }
                if (errorelipses_checkBox.Checked)
                {
                    draw_ellipse(Convert.ToDouble(m.getLatitude()), Convert.ToDouble(m.getLongitude()), Convert.ToDouble(m.getXerror()), Convert.ToDouble(m.getYerror()));                    
                }
                if(!errorelipses_checkBox.Checked)
                {
                    polygons.Polygons.Clear();
                }


               // gMap.Overlays.Add(markers);
                markers.Markers.Add(marker);
                              


            }
        }

       
        private MyMarker getCenter()
        {
            if (marker_List.Count() == 0)
            {
                MyMarker uofc = new MyMarker("center", "51.079948", "-114.125534", "0", "0");
                return uofc;
            }

            /*double sumX = 0;
            double sumY = 0;
            double sumZ = 0;

            int i;
            for (i = 0; i < marker_List.Count(); i++)
            {
                double x = 0;
                double y = 0;
                double z = 0;

                x = Math.Cos(Math.PI/180*Convert.ToDouble(marker_List[i].getLatitude())) * Math.Cos(Math.PI / 180 * Convert.ToDouble(marker_List[i].getLongitude()));
                y = Math.Cos(Math.PI / 180 * Convert.ToDouble(marker_List[i].getLatitude())) * Math.Sin(Math.PI / 180 * Convert.ToDouble(marker_List[i].getLongitude()));
                z = Math.Sin(Math.PI / 180 * Convert.ToDouble(marker_List[i].getLatitude()));

                sumX += x;
                sumY += y;
                sumZ += z;
            }

            sumX = sumX / i;
            sumY = sumY / i;
            sumZ = sumZ / i;

            double tempLat = 0;
            double tempLong = 0;

            tempLong = Math.Atan2(sumY, sumX);
            double tempVar = Math.Sqrt(sumX * sumX + sumY * sumY);
            tempLat = Math.Atan2(sumZ, tempVar);

            tempLong = tempLong * 180 / Math.PI;
            tempLat = tempLat * 180 / Math.PI;*/
            

            MyMarker temp = new MyMarker("center", marker_List[0].getLatitude(), marker_List[0].getLongitude(), "0", "0");
            return temp;
        }

        private void reCenter()
        {
            while (!killRecenter)
            {
                System.Threading.Thread.Sleep(5000);
                MyMarker averageCenter = getCenter();
                if (InvokeRequired)
                {
                    this.Invoke(new Action(() => gMap.SetPositionByKeywords(averageCenter.getLatitude() + "," + averageCenter.getLongitude())));

                }
            }


        }

        #endregion

        #region SOCKET
        public void startSocket()
        {
            try
            {
               
                IPAddress ipAddress = IPAddress.Parse("10.13.115.218");
              
                TcpListener tcpListener = new TcpListener(ipAddress, 8001);
                
                tcpListener.Start();
              
                Console.WriteLine("The server is running at port 8001...");
                Console.WriteLine("The local End point is  :" + tcpListener.LocalEndpoint);
                Console.WriteLine("Waiting for a connection.....");

                Socket socket = tcpListener.AcceptSocket();
                Console.WriteLine("Connection accepted from " + socket.RemoteEndPoint);

                while (!stopSocket)
                {
                    StringBuilder check = new StringBuilder("");
                    byte[] b = new byte[100];
                    int k = socket.Receive(b);
                    Console.WriteLine("Recieved...");
                    for (int i = 0; i < k; i++)
                    {
                        Console.Write(Convert.ToChar(b[i]));
                        check.Append(Convert.ToChar(b[i]));
                    }

                    String newData = check.ToString();
                    if (newData.Contains("DONE")) break;
                    
                    String[] dataElements = newData.Split(',');
                    markerID_text = dataElements[0];
                    lat_text = dataElements[1];
                    long_text = dataElements[2];
                    latError_text = dataElements[3];
                    longError_text = dataElements[4];

                    if (InvokeRequired)
                    {
                        this.BeginInvoke(new Action(() => updateMarkerList()));
                        this.BeginInvoke(new Action(() => drawMarkers()));
                        this.BeginInvoke(new Action(() => updateTable()));
                    }
                                       
                    
                    


                    ASCIIEncoding asen = new ASCIIEncoding();
                    socket.Send(asen.GetBytes("The string was recieved by the server."));
                    Console.WriteLine("\nSent Acknowledgement");


                }

                ASCIIEncoding fin = new ASCIIEncoding();
                socket.Send(fin.GetBytes("DONE"));
                socket.Close();
                tcpListener.Stop();
                Console.Write("Finished...\n");
            }
            catch (Exception e)
            {
                Console.WriteLine("Error..... " + e.StackTrace);
            }
        }


        #endregion


        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            stopSocket = true;                    
            if (socketThread.IsAlive) socketThread.Join();      
            Environment.Exit(0);
        }



        

    }


    public class MyMarker
    {
        private string markerID;
        private string lat;
        private string longi;
        private string xerr;
        private string yerr;

        public MyMarker(string ID, string lat, string longi, string xerr, string yerr)
        {
            this.markerID = ID;
            this.lat = lat;
            this.longi = longi;
            this.xerr = xerr;
            this.yerr = yerr;
        }

        public void update(string lat, string longi, string xerr, string yerr)
        {
            setLatitude(lat);
            setLongitude(longi);
            setXerror(xerr);
            setYerror(yerr);
        }

        public string getMarkerID()
        {
            return markerID;
        }
        public string getLatitude()
        {
            return lat;
        }
        public string getLongitude()
        {
            return longi;
        }
        public string getXerror()
        {
            return xerr;
        }
        public string getYerror()
        {
            return yerr;
        }

        public void setMarkerID(string ID)
        {
            this.markerID = ID;
        }
        public void setLatitude(string lat)
        {
            this.lat = lat;
        }
        public void setLongitude(string longi)
        {
            this.longi = longi;
        }
        public void setXerror(string xerr)
        {
            this.xerr = xerr;
        }
        public void setYerror(string yerr)
        {
            this.yerr = yerr;
        }

    }
}


