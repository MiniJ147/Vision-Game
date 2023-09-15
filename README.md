<h1 algin="left">Vision Game</h1>
<h3>Table of Contents</h3>
<h4 algin="left"><h4>
<p>
  <ul>
    <li><a href="#desc">Description</a></li>
      <ul>
        <li><a href="#socials">Socials</a></li>
        <li><a href="#tech_info">Tech Info</a></li>
      </ul>
    <li><a href="#docs">Documentation</a></li>
  </ul>
</p>

  
<h3 algin="left">About</h3>
<p>
  <h4 algin="left" id="desc">Desc:</h4>
  Small single player horror game that I am working on. This project came about due to a nightmare I had, 
  and after I woke up I relized it would make an amazing game.<br><br>

  <h4 id="socials">Socials</h4>
   <a target="_blank" href="https://www.youtube.com/@Mini_CS">YouTube</a> --> Where I live stream all of development<br>
   <a target="_blank" href="https://trello.com/b/8FdX8EIt/horror-game">Roadmap</a> --> General Idea of Process & future
  
  <h4 algin="left" id="tech_info">Tech Info</h4>
  <ul>
    <li>Programmed in C style C++</li>
    <li>DOD Design Structure</li>
    <li>Renderer Opengl</li>
  </ul>
</p>


<!-- 
What is the point of docs? 
For me to read back on and maybe others when they are looking through the code base

What to document?
-Player
-Monster
-Map system
-->
<h1 id="docs">Documentation</h1>
<h3 algin="left">Table of Contents<h3>
<p>
  <ul> <!-- Least to most complex Systems !-->
    <li><a href="#player">Player</li> 
    <li><a href="#rendering">Rendering</li>
    <li><a href="#map_system">Map System</li>
    <li><a href="#monster">Monster</li>
  </ul>
</p>

<h2 id="player">Player</h2>

<h3>Player Create(Player* player,  float x, float y,  const char* texture_path)</h3>
&nbsp;Initializes basic player info, such as positions and textures. Then centers world camera around the player.

<br><h3>Tick(Vec2f* pos, Map m, float move_speed, float delta_time)</h3>
&nbsp;Allows player to move every frame.

<br><h3>void collision_check_x(int dir, Vec2f* pos, Map m, float move_speed, float delta_time) //and y</h3>
<h4>steps</h4>
<ol>
  <li>Adjust to 32 x 32 cell matrix</li>
  <li>Reaches into block it is going to collide with and checks collision flag of block</li>
  <li>If it is colliable it prevents future movement</li>
</ol>

<h4>Special Note</h4>&nbsp;&nbsp;
For y if player is in 2 cells simultaneously it must check bottom left and bottom right<br>&nbsp;&nbsp;
For x if player is in 2 cells simultaneosuly it must check top right and bottom right (right or 
depending on direction)<br><br>

![expl2](https://github.com/MiniJ147/Vision-Game/assets/47308554/624990f8-dd82-4e84-95af-793839429a5a)
![explination](https://github.com/MiniJ147/Vision-Game/assets/47308554/ac13c2ad-811a-437e-a66f-127ded26df29)


<br><br><h2 id="rendering">Rendering</h2>
<br><br><h2 id="map_system">Map System</h2>
<br><br><h2 id="monster">Monster</h2>
