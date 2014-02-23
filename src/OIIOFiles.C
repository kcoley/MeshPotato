
#include "MPUtils/OIIOFiles.h"
#include <OpenEXR/half.h>
#include <iostream>
#include <cmath>
#include <OpenImageIO/imageio.h> 
#include <list>
#include <ctime>
#include <sstream>
#include <pwd.h>
#include <sys/utsname.h>

using namespace std;
namespace MeshPotato {
namespace MPUtils {
OIIO_NAMESPACE_USING


float imagePlaneValue( float v, float dG, float dB )
{
	if( v == 0.0 ){ return 0.0; }
	return pow(v, dG) * dB;
}


template <typename T> 
std::string oiiotostr(const T& t) { std::stringstream os; os<<t; return os.str(); }


void writeOIIOImage( const char* fname, Image& img, float displayBrightness, float displayGamma  )
{
	float* imagedata = new float[ img.Width()* img.Height() * img.Depth() ];

	// fill image with the contents of img

	long index = 0;
	for( int j=0;j<img.Height();j++ )
	{
		for( int i=0;i<img.Width();i++ )
		{
			vector<float> pix = img.pixel(i,img.Height() - j - 1);
			for( size_t c=0;c<(size_t)img.Depth();c++ )
			{
				if(c<3){ pix[c] = imagePlaneValue( pix[c], displayGamma, displayBrightness ); }
				imagedata[index++] = pix[c];
			}
		}
	}

	ImageOutput *out = ImageOutput::create (fname); 
	if( !out )
	{
		cout << "Not able to write an image to file " << fname << endl;
	}
	else
	{
		ImageSpec spec (img.Width(), img.Height(), img.Depth(), TypeDesc::FLOAT);
		spec.channelnames.clear ();
		if( img.Depth() > 0 ){ spec.channelnames.push_back ("R"); }
		if( img.Depth() > 1 ){ spec.channelnames.push_back ("G"); }
		if( img.Depth() > 2 ){ spec.channelnames.push_back ("B"); }
		if( img.Depth() > 3 ){ spec.channelnames.push_back ("A"); spec.alpha_channel = 3; }
		if( img.Depth() > 4 ){ spec.channelnames.push_back ("Z"); spec.z_channel = 4; }
		for( int i=5;i<img.Depth();i++)
		{
			string channelname = "C" + oiiotostr(i);
			spec.channelnames.push_back(channelname);
		}
		spec.attribute("ImageDescription", "" );
		spec.attribute("Keywords", "" );
		register struct passwd *pw;
		register uid_t uid;
		uid = geteuid ();
		pw = getpwuid (uid);
		string artist = "Unknown";
		if (pw)
		{
			artist = string(pw->pw_name);
		}
		spec.attribute("Artist", artist );
		spec.attribute("Copyright", "" );
		spec.attribute("DateTime", "" );
		spec.attribute("DocumentName", "" );
		spec.attribute("Software", "MeshPotato" );
		string hostcomputer = "Unknown";
		struct utsname u_name;
		int z = uname(&u_name);
		if ( z != -1 ) 
		{
			hostcomputer = string( u_name.nodename );
		}
		spec.attribute("HostComputer", hostcomputer );
		string datetime = "";
		time_t t = time(NULL);
		struct tm tm = *localtime(&t);
		datetime = oiiotostr(tm.tm_year + 1900) + "-" + oiiotostr(tm.tm_mon + 1) + "-" + oiiotostr(tm.tm_mday) + " " + oiiotostr(tm.tm_hour) + ":" +  oiiotostr(tm.tm_min) + ":" + oiiotostr(tm.tm_sec);
		spec.attribute("DateTime", datetime );
		out->open (fname, spec);
		out->write_image (TypeDesc::FLOAT, imagedata); 
		out->close (); 
		delete out;
	}
	delete[] imagedata;
}


void writeOIIOImage( const char* fname, Image& img, const vector<string>& keys, const vector<string>& tags,  float displayBrightness, float displayGamma )
{
	map<string,string> labels;
	size_t nblabels = keys.size();
	nblabels = ( nblabels < tags.size() )? nblabels : tags.size();
	for( size_t i=0;i<nblabels;i++ )
	{
		labels[ keys[i] ] = tags[i];
	}
	writeOIIOImage( fname, img, labels, displayBrightness, displayGamma );

}
void writeOIIOImage( const char* fname, DeepImage& img, const vector<string>& keys, const vector<string>& tags,  float displayBrightness, float displayGamma )
{
	map<string,string> labels;
	size_t nblabels = keys.size();
	nblabels = ( nblabels < tags.size() )? nblabels : tags.size();
	for( size_t i=0;i<nblabels;i++ )
	{
		labels[ keys[i] ] = tags[i];
	}
	writeOIIOImage( fname, img, labels, displayBrightness, displayGamma );

}


void writeOIIOImage( const char* fname, Image& img, const map<string,string>& labels, float displayBrightness, float displayGamma )
{
	float* imagedata = new float[ img.Width()* img.Height() * img.Depth() ];

	// fill image with the contents of img

	long index = 0;
	for( int j=0;j<img.Height();j++ )
	{
		for( int i=0;i<img.Width();i++ )
		{
			vector<float> pix = img.pixel(i,img.Height() - j - 1);
			for( size_t c=0;c<(size_t)img.Depth();c++ )
			{
				if(c<3){ pix[c] = imagePlaneValue( pix[c], displayGamma, displayBrightness ); }
				imagedata[index++] = pix[c];
			}
		}
	}

	ImageOutput *out = ImageOutput::create (fname); 
	if( !out )
	{
		cout << "Not able to write an image to file " << fname << endl;
	}
	else
	{
		ImageSpec spec (img.Width(), img.Height(), img.Depth(), TypeDesc::FLOAT);
		spec.channelnames.clear ();
		if( img.Depth() > 0 ){ spec.channelnames.push_back ("R"); }
		if( img.Depth() > 1 ){ spec.channelnames.push_back ("G"); }
		if( img.Depth() > 2 ){ spec.channelnames.push_back ("B"); }
		if( img.Depth() > 3 ){ spec.channelnames.push_back ("A"); spec.alpha_channel = 3; }
		if( img.Depth() > 4 ){ spec.channelnames.push_back ("Z"); spec.z_channel = 4; }
		for( int i=5;i<img.Depth();i++)
		{
			string channelname = "C" + oiiotostr(i);
			spec.channelnames.push_back(channelname);
		}

		spec.attribute("ImageDescription", "" );
		spec.attribute("Keywords", "" );
		register struct passwd *pw;
		register uid_t uid;
		uid = geteuid ();
		pw = getpwuid (uid);
		string artist = "Unknown";
		if (pw)
		{
			artist = string(pw->pw_name);
		}
		spec.attribute("Artist", artist );
		spec.attribute("Copyright", "" );
		spec.attribute("DateTime", "" );
		spec.attribute("DocumentName", "" );
		spec.attribute("Software", "imageTools" );
		string hostcomputer = "Unknown";
		struct utsname u_name;
		int z = uname(&u_name);
		if ( z != -1 ) 
		{
			hostcomputer = string( u_name.nodename );
		}
		spec.attribute("HostComputer", hostcomputer );
		string datetime = "";
		time_t t = time(NULL);
		struct tm tm = *localtime(&t);
		datetime = oiiotostr(tm.tm_year + 1900) + "-" + oiiotostr(tm.tm_mon + 1) + "-" + oiiotostr(tm.tm_mday) + " " + oiiotostr(tm.tm_hour) + ":" +  oiiotostr(tm.tm_min) + ":" + oiiotostr(tm.tm_sec);
		spec.attribute("DateTime", datetime );

		if( labels.size() > 0 )
		{
			map<string,string>::const_iterator lab = labels.begin();
			while( lab != labels.end() )
			{
				const string& name = lab->first;
				const string& value = lab->second;
				spec.attribute( name, value );
				lab++;
			}
		}
		out->open (fname, spec);
		out->write_image (TypeDesc::FLOAT, imagedata); 
		out->close (); 
		delete out;
	}
	delete[] imagedata;
}

void writeOIIOImage( const char* fname, DeepImage& img, float brightness, float gamma ) {
	// Prepare the spec for ’half’ RGBA, ’float’ z
	int xres = img.Width(), yres = img.Height();
	int nchannels = img.Depth();
	ImageSpec spec (xres, yres, nchannels);
	TypeDesc channeltypes[] = { TypeDesc::HALF, TypeDesc::HALF,
		TypeDesc::HALF, TypeDesc::HALF, TypeDesc::FLOAT, TypeDesc::FLOAT };
	spec.z_channel = 4;
	spec.channelnames[spec.z_channel] = "Z";
	spec.channelformats.assign (channeltypes+0, channeltypes+nchannels);
	spec.deep = true;
	DeepData deepdata;
	spec.attribute("ImageDescription", "Deep Image" );
	spec.attribute("Keywords", "" );
	register struct passwd *pw;
	register uid_t uid;
	uid = geteuid ();
	pw = getpwuid (uid);
	string artist = "Unknown";
	if (pw)
	{
		artist = string(pw->pw_name);
	}
	spec.attribute("Artist", artist );
	spec.attribute("Copyright", "" );
	spec.attribute("DateTime", "" );
	spec.attribute("DocumentName", "" );
	spec.attribute("Software", "MeshPotato" );
	string hostcomputer = "Unknown";
	struct utsname u_name;
	int z = uname(&u_name);
	if ( z != -1 ) 
	{
		hostcomputer = string( u_name.nodename );
	}
	spec.attribute("HostComputer", hostcomputer );
	string datetime = "";
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	datetime = oiiotostr(tm.tm_year + 1900) + "-" + oiiotostr(tm.tm_mon + 1) + "-" + oiiotostr(tm.tm_mday) + " " + oiiotostr(tm.tm_hour) + ":" +  oiiotostr(tm.tm_min) + ":" + oiiotostr(tm.tm_sec);
	spec.attribute("DateTime", datetime );

	deepdata.init (xres*yres, nchannels, channeltypes+0, channeltypes+nchannels);
	for (int y = 0; y < yres; ++y)
		for (int x = 0; x < xres; ++x)
			deepdata.nsamples[y*xres+x] = img.value(x,img.Height() - y - 1).size();
	deepdata.alloc (); // allocate pointers and data
	int pixel = 0;
	for (int y = 0; y < yres; ++y) {
		for (int x = 0; x < xres; ++x, ++pixel) {
			DeepPixelBufferVector pixels = img.getDeepPixelBufferVector(x,img.Height() - y - 1);
			for (int chan = 0; chan < nchannels; ++chan) {
				void *ptr = deepdata.pointers[pixel*nchannels + chan];
				if (chan < 4) { // RGBA -- it’s HALF data
					for (int samp = 0; samp < deepdata.nsamples[pixel]; ++samp) {
						((half *)ptr)[samp] = pixels[samp].color[chan];
					}
				} else {
					// z channel -- FLOAT data
					for (int samp = 0; samp < deepdata.nsamples[pixel]; ++samp)
						((float *)ptr)[samp] = pixels[samp].depth_front;//...value...;
				}
			}
		}
	}


	// Create the output
	std::string error;
	ImageOutput *out = ImageOutput::create (fname);
	if (! out) {
		error = geterror();
		if (error.empty())
			error = Strutil::format("Could not open");
		std::cout << error << std::endl;
		return;
	}
	// Make sure the format can handle deep data and per-channel formats
	std::cout << "output image" << std::endl;
	// Do the I/O (this is the easy part!)
	bool worked = out->open (fname, spec);
	if (!worked) {
		std::string error = geterror();
		if (error.empty())
			std::cout << "didn't work" << std::endl;
		else
			std::cout << error << std::endl;
	}
	out->write_deep_image (deepdata);
	out->close ();
	delete out;

}
void writeOIIOImage( const char* fname, DeepImage& img, const map<string,string>& labels, float brightness, float gamma ) {
	// Prepare the spec for ’half’ RGBA, ’float’ z
	int xres = img.Width(), yres = img.Height();
	int nchannels = img.Depth();
	ImageSpec spec (xres, yres, nchannels);
	TypeDesc channeltypes[] = { TypeDesc::HALF, TypeDesc::HALF,
		TypeDesc::HALF, TypeDesc::HALF, TypeDesc::FLOAT, TypeDesc::FLOAT };
	spec.z_channel = 4;
	spec.channelnames[spec.z_channel] = "Z";
	spec.channelformats.assign (channeltypes+0, channeltypes+nchannels);
	spec.deep = true;
	DeepData deepdata;
	spec.attribute("ImageDescription", "Deep Image" );
	spec.attribute("Keywords", "" );
	register struct passwd *pw;
	register uid_t uid;
	uid = geteuid ();
	pw = getpwuid (uid);
	string artist = "Unknown";
	if (pw)
	{
		artist = string(pw->pw_name);
	}
	spec.attribute("Artist", artist );
	spec.attribute("Copyright", "" );
	spec.attribute("DateTime", "" );
	spec.attribute("DocumentName", "" );
	spec.attribute("Software", "MeshPotato" );
	string hostcomputer = "Unknown";
	struct utsname u_name;
	int z = uname(&u_name);
	if ( z != -1 ) 
	{
		hostcomputer = string( u_name.nodename );
	}
	spec.attribute("HostComputer", hostcomputer );
	string datetime = "";
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	datetime = oiiotostr(tm.tm_year + 1900) + "-" + oiiotostr(tm.tm_mon + 1) + "-" + oiiotostr(tm.tm_mday) + " " + oiiotostr(tm.tm_hour) + ":" +  oiiotostr(tm.tm_min) + ":" + oiiotostr(tm.tm_sec);
	spec.attribute("DateTime", datetime );
	if (labels.size() > 0) {
		map<string,string>::const_iterator lab = labels.begin();
		while (lab != labels.end()) {
		        const string& name = lab->first;
                        const string& value = lab->second;
                        spec.attribute( name, value );
                        lab++;
		}
	}




	deepdata.init (xres*yres, nchannels, channeltypes+0, channeltypes+nchannels);
	for (int y = 0; y < yres; ++y)
		for (int x = 0; x < xres; ++x)
			deepdata.nsamples[y*xres+x] = img.value(x,img.Height() - y - 1).size();
	deepdata.alloc (); // allocate pointers and data
	int pixel = 0;
	for (int y = 0; y < yres; ++y) {
		for (int x = 0; x < xres; ++x, ++pixel) {
			DeepPixelBufferVector pixels = img.getDeepPixelBufferVector(x,img.Height() - y - 1);
			for (int chan = 0; chan < nchannels; ++chan) {
				void *ptr = deepdata.pointers[pixel*nchannels + chan];
				if (chan < 4) { // RGBA -- it’s HALF data
					for (int samp = 0; samp < deepdata.nsamples[pixel]; ++samp) {
						((half *)ptr)[samp] = pixels[samp].color[chan];
					}
				} else {
					// z channel -- FLOAT data
					for (int samp = 0; samp < deepdata.nsamples[pixel]; ++samp)
						((float *)ptr)[samp] = pixels[samp].depth_front;//...value...;
				}
			}
		}
	}


	// Create the output
	std::string error;
	ImageOutput *out = ImageOutput::create (fname);
	if (! out) {
		error = geterror();
		if (error.empty())
			error = Strutil::format("Could not open");
		std::cout << error << std::endl;
		return;
	}
	// Make sure the format can handle deep data and per-channel formats
	std::cout << "output image" << std::endl;
	// Do the I/O (this is the easy part!)
	bool worked = out->open (fname, spec);
	if (!worked) {
		std::string error = geterror();
		if (error.empty())
			std::cout << "didn't work" << std::endl;
		else
			std::cout << error << std::endl;
	}
	out->write_deep_image (deepdata);
	out->close ();
	delete out;

}




void readOIIOImage( const char* fname, Image& img  )
{
	int xres, yres, channels;
	ImageInput *in = ImageInput::create (fname);
	if (! in) {return;}
	ImageSpec spec;
	in->open (fname, spec);
	xres = spec.width;
	yres = spec.height;
	channels = spec.nchannels;
	float* pixels = new float[xres*yres*channels];
	in->read_image (TypeDesc::FLOAT, pixels);

	img.reset( xres, yres, channels );
	long index = 0;
	for( int j=0;j<yres;j++)
	{
		for( int i=0;i<xres;i++ )
		{
			for( int c=0;c<channels;c++ )
			{
				img.value(i,img.Height() - j - 1,c) = pixels[index++];
			}
		}
	}
	in->close ();
	delete in;
	delete[] pixels;
}


void readOIIOImage( const char* fname, Image& img, map<string,string>& labels )
{
	int xres, yres, channels;
	ImageInput *in = ImageInput::create (fname);
	if (! in) {return;}
	ImageSpec spec;
	in->open (fname, spec);
	xres = spec.width;
	yres = spec.height;
	channels = spec.nchannels;
	float* pixels = new float[xres*yres*channels];
	in->read_image (TypeDesc::FLOAT, pixels);

	img.reset( xres, yres, channels );
	long index = 0;
	for( int j=0;j<yres;j++)
	{
		for( int i=0;i<xres;i++ )
		{
			for( int c=0;c<channels;c++ )
			{
				img.value(i,img.Height() - j - 1,c) = pixels[index++];
			}
		}
	}

	for( size_t i=0;i<spec.extra_attribs.size();i++)
	{
		const ParamValue& p = spec.extra_attribs[i];
		string name = p.name().c_str();
		string value = spec.metadata_val ( p, true);
		labels[name] = value;
	}
	labels["ImageDimensions"] = "" + oiiotostr(xres) + " X " + oiiotostr(yres) + " X " + oiiotostr(channels);

	for( size_t i=0;i<spec.channelnames.size();i++ )
	{
		labels["Channel " + oiiotostr(i) + " Name"] = spec.channelnames[i];
	}
	if( spec.alpha_channel >= 0 && spec.alpha_channel < spec.channelnames.size() )
	{ 
		labels["AlphaChannel"] = spec.channelnames[spec.alpha_channel];
	}
	if( spec.z_channel >= 0 && spec.z_channel < spec.channelnames.size() )
	{ 
		labels["ZChannel"] = spec.channelnames[spec.z_channel];
	}

	in->close ();
	delete in;
	delete[] pixels;
}



void readOIIOImage( const char* fname, Image& img, vector<string>& keys, vector<string>& tags )
{
	map<string,string> labels;
	readOIIOImage( fname, img, labels );
	map<string,string>::iterator p = labels.begin();
	keys.clear();
	tags.clear();
	while( p != labels.end() )
	{
		keys.push_back( p->first );
		tags.push_back( p->second );
		p++;
	}
}




void printMetadata( const map<string,string>& meta )
{
	if( meta.empty() ){ return; }
	cout << "\n\nMetadata Labels\n==============================\n";
	map<string,string>::const_iterator p = meta.begin();
	size_t maxlength = 0;
	while( p != meta.end() )
	{
		maxlength = ( maxlength < p->first.size() ) ? p->first.size() : maxlength;
		p++;
	}
	maxlength += 1;
	p = meta.begin();
	while( p != meta.end() )
	{
		size_t extraspace = maxlength - p->first.size();
		cout << p->first;
		for( size_t i=0;i<extraspace;i++ ){ cout << " "; }
		cout << "---------> " << p->second << endl;
		p++;
	}
	cout << "\n==============================\n\n";

}



float* convert( const MeshPotato::MPUtils::Image& img, int channel )
{
	float* data = new float[img.Width() * img.Height() ];
	long index = 0;
	for( int y=0;y<img.Height();y++)
	{
		for( int x=0;x<img.Width();x++ )
		{
			data[index++] = img.value(x,y,channel);
		}
	}
	return data;
}

}}
