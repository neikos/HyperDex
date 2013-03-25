package hyperclient;

import java.util.*;
import java.nio.*;

public class DeferredSearchDescribe extends Deferred
{
    private int unsafe;

	private String[] description = { "" }; // Single element will be filled with
										   // ASCII description. Should be client's	
										   // current default encoding in the future.

    public DeferredSearchDescribe(HyperClient client, Object space, Map predicate, boolean unsafe)
                                                            throws HyperClientException,
                                                                   TypeError,
                                                                   ValueError,
                                                                   MemoryError
    {
        super(client);

        this.unsafe = unsafe?1:0;

        if ( predicate == null )
            throw new ValueError("DeferredSearchDescribe critera cannot be null");

        hyperclient_attribute_check chks = null;
        long chks_sz = 0;

        try
        {
            Vector retvals = client.predicate_to_c(predicate);
            
            chks = (hyperclient_attribute_check)(retvals.get(0));
            chks_sz = ((Long)(retvals.get(1))).longValue();

            reqId = client.search_describe(client.getBytes(space,true),
                                 chks, chks_sz,
                                 rc_ptr,
                                 description);

            checkReqIdSearch(reqId, status(), chks, chks_sz);

            client.ops.put(reqId,this);
        }
        finally
        {
            if ( chks != null ) HyperClient.free_attrs_check(chks, chks_sz);
        }
    }

    public Object waitFor() throws HyperClientException, ValueError, TypeError
    {
        super.waitFor();

        if (status() == hyperclient_returncode.HYPERCLIENT_SUCCESS || unsafe == 0)
        {
           	return ByteArray.encode(description[0],"UTF-8"); // "UTF-8" should be
														     // replaced by
														     // client.getDefaultEncoding
														     // in the future. If the
															 // description is ASCII
															 // than "UTF-8" will work.
        }
        else
        {
            throw new HyperClientException(status());
        }
    }
}
