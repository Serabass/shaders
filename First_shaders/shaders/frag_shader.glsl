uniform float time;
uniform vec2 resolution;

void main( void ) {

	vec2 position = ( gl_FragCoord.xy / resolution.xy ) / 4.0;

	float color = 0.0;

	vec4 res = vec4( int(mod(gl_FragCoord.x*1.75, 2.))-int( mod(gl_FragCoord.y*1.75, 2.)), int(mod(gl_FragCoord.x*1.75, 2.))-int( mod(gl_FragCoord.y*1.75, 2.)), int(mod(gl_FragCoord.x*1.75, 2.))-int( mod(gl_FragCoord.y*1.75, 2.)), 1.0 );
	res.r -= abs(sin(gl_FragCoord.x/50.+time));
	res.b -= abs(cos(gl_FragCoord.y/50.+time));
	res.g -= abs(tan((gl_FragCoord.x + gl_FragCoord.y)/150.-time));
	gl_FragColor = res;
}