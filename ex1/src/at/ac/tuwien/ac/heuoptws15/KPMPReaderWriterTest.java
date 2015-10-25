package at.ac.tuwien.ac.heuoptws15;

import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.List;

public class KPMPReaderWriterTest {

	public static void main(String[] args) throws FileNotFoundException {
		System.out.println(System.getProperty("user.dir"));
		KPMPInstance inst = KPMPInstance.readInstance(args[0]);
		KPMPSolutionWriter writer = new KPMPSolutionWriter(inst.getK());
		
		List<Integer> spineOrder = new ArrayList<>();
		for(int i=0; i<inst.getNumVertices(); ++i) {
			spineOrder.add(i);
		}
		writer.setSpineOrder(spineOrder);
		
		for(int a=0; a<inst.getNumVertices(); ++a) {
			for(int b: inst.getAdjacencyList().get(a)) {
				if(b>=a) continue;
				
				writer.addEdgeOnPage(a, b, 0);
			}
		}
		
		writer.print();
	}

}
